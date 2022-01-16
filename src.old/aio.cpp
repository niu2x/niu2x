#include <niu2x/aio.h>

#include <string.h>

#include <uv.h>

#define LOOP uv_default_loop()

namespace nx::aio {

namespace {

mallocfree_memory default_memory;
memory_proxy default_allocator { &default_memory };

rid next_rid = 0;

struct idle_item {
    rid id;
    uv_idle_t uv_obj;
    idle_handle callback;
    list_t list;
};

struct tcp_connect_item;
struct tcp_item {
    rid id;
    uv_tcp_t uv_obj;
    list_t list;
    tcp_connect_item* con;
    tcp_read_callback read_cb;
    tcp_connect_callback client_connect_cb;
    bool ready;
    bool reading;
};

struct tcp_connect_item {
    rid id;
    tcp_item* tcp;
    list_t list;
    uv_connect_t uv_obj;
    struct sockaddr_in dest;
    tcp_connect_callback callback;
};

struct stream_write_req {
    uv_write_t req;
    uv_buf_t buf;
    tcp_write_callback callback;
};

NX_LIST(idles);
NX_LIST(tcps);
NX_LIST(tcp_connects);

#define DEF_FIND_FUNC(elem)                                                    \
    static elem##_item* find_##elem##_item(rid elem##_id, list_head* head)     \
    {                                                                          \
        elem##_item* item = nullptr;                                           \
        NX_LIST_FOR_EACH(ptr, head)                                            \
        {                                                                      \
            item = NX_LIST_ENTRY(ptr, struct elem##_item, list);               \
            if (item->id == elem##_id)                                         \
                return item;                                                   \
        }                                                                      \
        return nullptr;                                                        \
    }

DEF_FIND_FUNC(idle)
DEF_FIND_FUNC(tcp)
DEF_FIND_FUNC(tcp_connect)

} // namespace

void run(int type)
{
    switch (type) {
        case wait_all: {
            uv_run(LOOP, UV_RUN_DEFAULT);
            break;
        }
        case wait_once: {
            uv_run(LOOP, UV_RUN_ONCE);
            break;
        }
        case no_wait: {
            uv_run(LOOP, UV_RUN_NOWAIT);
            break;
        }
    }
}

void set_allocator(memory_proxy allocator) { default_allocator = allocator; }

static void idle_callback(uv_idle_t* handle)
{
    auto* idle = reinterpret_cast<idle_item*>(handle->data);
    idle->callback(idle->id);
}

rid create_idle(const idle_handle& handler)
{
    NX_LOG_D("aio create_idle");

    auto* idle = (idle_item*)default_allocator.allocate(sizeof(idle_item));
    new (idle) idle_item;

    idle->id = next_rid++;
    idle->callback = handler;
    list_add_tail(&(idle->list), &idles);
    uv_idle_init(LOOP, &(idle->uv_obj));
    idle->uv_obj.data = idle;
    uv_idle_start(&(idle->uv_obj), idle_callback);
    return idle->id;
}

void destroy_idle(rid idle_id)
{
    auto* idle = find_idle_item(idle_id, &idles);
    NX_ASSERT(idle, "idle_id isn't exist");

    uv_idle_stop(&(idle->uv_obj));
    uv_close(reinterpret_cast<uv_handle_t*>(&(idle->uv_obj)), nullptr);
    list_del(&(idle->list));

    idle->~idle_item();
    default_allocator.free(idle);

    NX_LOG_D("aio destroy_idle");
}

rid create_tcp()
{
    NX_LOG_D("aio create_tcp");
    auto* tcp = (tcp_item*)default_allocator.allocate(sizeof(tcp_item));
    new (tcp) tcp_item;
    tcp->id = next_rid++;
    list_add_tail(&(tcp->list), &tcps);
    uv_tcp_init(LOOP, &(tcp->uv_obj));
    tcp->uv_obj.data = tcp;
    tcp->ready = false;
    return tcp->id;
}

void destroy_tcp(rid tcp_id)
{
    auto* tcp = find_tcp_item(tcp_id, &tcps);
    NX_ASSERT(tcp, "tcp_id isn't exist");
    uv_close(reinterpret_cast<uv_handle_t*>(&(tcp->uv_obj)), nullptr);
    list_del(&(tcp->list));
    tcp->~tcp_item();
    default_allocator.free(tcp);
    NX_LOG_D("aio destroy_tcp");
}

static void destroy_tcp_connect(rid con_id)
{
    auto* con = find_tcp_connect_item(con_id, &tcp_connects);
    NX_ASSERT(con, "con_id isn't exist");
    list_del(&(con->list));
    con->~tcp_connect_item();
    default_allocator.free(con);
    NX_LOG_D("aio destroy_tcp_connect");
}

static void uv_tcp_connect_callback(uv_connect_t* uv_con, int status)
{
    auto* con = reinterpret_cast<tcp_connect_item*>(uv_con->data);
    auto* tcp = con->tcp;
    tcp->con = nullptr;

    char* address = inet_ntoa(con->dest.sin_addr);

    if (status < 0) {
        NX_LOG_E("tcp failed to connect: %s:%u reason: %s", address,
            ntohs(con->dest.sin_port), uv_strerror(status));
        con->callback(fail, tcp->id);
    } else {
        NX_LOG_D("tcp connect to %s:%u", address, ntohs(con->dest.sin_port));
        tcp->ready = true;
        con->callback(ok, tcp->id);
    }

    destroy_tcp_connect(con->id);
}

static bool create_tcp_connect(rid& con_id, tcp_item* tcp, const char* ip,
    uint16_t port, const tcp_connect_callback& cb)
{
    NX_LOG_D("aio create_tcp_connect %s %u", ip, port);
    NX_ASSERT(tcp->con == nullptr && tcp->ready == false, "");
    auto* con = (tcp_connect_item*)default_allocator.allocate(
        sizeof(tcp_connect_item));
    new (con) tcp_connect_item;
    con->id = next_rid++;
    list_add_tail(&(con->list), &tcp_connects);
    con->uv_obj.data = con;
    tcp->con = con;
    con->tcp = tcp;
    con->callback = cb;

    auto* addr = &(con->dest);

    uv_ip4_addr(ip, port, addr);
    con_id = con->id;

    bool success = !uv_tcp_connect(&(con->uv_obj), &(tcp->uv_obj),
        (const struct sockaddr*)(addr), uv_tcp_connect_callback);
    return success;
}

void tcp_connect(
    rid tcp_id, const char* ip, uint16_t port, const tcp_connect_callback& cb)
{
    auto* tcp = find_tcp_item(tcp_id, &tcps);
    NX_ASSERT(tcp, "tcp_id isn't exist");

    rid con_id;
    if (!create_tcp_connect(con_id, tcp, ip, port, cb)) {
        destroy_tcp_connect(con_id);
        create_idle([tcp_id, cb](auto self) {
            destroy_idle(self);
            cb(fail, tcp_id);
        });
    }
}

static void uv_tcp_new_connection_callback(uv_stream_t* server, int status)
{
    auto* tcp = reinterpret_cast<tcp_item*>((server->data));
    if (status < 0) {
        NX_LOG_E("New connection error %s\n", uv_strerror(status));
        tcp->client_connect_cb(fail, tcp->id);
        return;
    }

    rid client_id = create_tcp();
    auto* client = find_tcp_item(client_id, &tcps);
    if (uv_accept(server, (uv_stream_t*)&(client->uv_obj)) == 0) {
        tcp->client_connect_cb(ok, client_id);
    } else {
        destroy_tcp(client_id);
        tcp->client_connect_cb(fail, tcp->id);
    }
}

void tcp_listen(
    rid tcp_id, const char* ip, uint16_t port, const tcp_connect_callback& cb)
{
    auto* tcp = find_tcp_item(tcp_id, &tcps);
    NX_ASSERT(tcp, "tcp_id isn't exist");

    struct sockaddr_in addr;
    uv_ip4_addr(ip, port, &addr);
    uv_tcp_bind(&(tcp->uv_obj), (const struct sockaddr*)&addr, 0);
    tcp->client_connect_cb = cb;

    int r = uv_listen(
        (uv_stream_t*)&(tcp->uv_obj), 4, uv_tcp_new_connection_callback);
    if (r) {
        NX_LOG_E("Listen error %s\n", uv_strerror(r));
        create_idle([tcp_id, cb](auto self) {
            destroy_idle(self);
            cb(fail, tcp_id);
        });
        return;
    }
}

static void alloc_buf(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
    unused(handle);
    buf->base = (char*)default_allocator.allocate(suggested_size);
    buf->len = (size_t)(buf->base ? suggested_size : 0);
}

static void free_buf(const uv_buf_t* buf)
{
    if (buf->base && buf->len) {
        default_allocator.free(buf->base);
    }
}

static void uv_tcp_read_callback(
    uv_stream_t* uv_stream, ssize_t nread, const uv_buf_t* buf)
{

    auto* tcp = reinterpret_cast<tcp_item*>((uv_stream->data));

    if (nread < 0) {
        tcp->read_cb(fail, tcp->id, nullptr, 0);
    }

    else if (nread > 0) {
        if (tcp->read_cb)
            tcp->read_cb(ok, tcp->id, (const uint8_t*)buf->base, nread);
    }

    free_buf(buf);
}

void tcp_read_start(rid tcp_id, const tcp_read_callback& cb)
{
    NX_LOG_D("tcp_read_start id: %lu", tcp_id);

    auto* tcp = find_tcp_item(tcp_id, &tcps);
    NX_ASSERT(tcp != nullptr, "");
    NX_ASSERT(tcp->reading == false, "");

    tcp->read_cb = cb;
    auto s = uv_read_start(
        (uv_stream_t*)&(tcp->uv_obj), alloc_buf, uv_tcp_read_callback);
    if (s) {

        create_idle([tcp_id, cb](auto self) {
            destroy_idle(self);
            cb(fail, tcp_id, nullptr, 0);
        });
        create_idle([tcp_id, cb](auto self) {
            destroy_idle(self);
            cb(fail, tcp_id, nullptr, 0);
        });
    } else {
        tcp->reading = true;
    }
}
void tcp_read_stop(rid tcp_id)
{
    NX_LOG_D("tcp_read_stop id: %lu", tcp_id);
    auto* tcp = find_tcp_item(tcp_id, &tcps);
    if (tcp->reading) {
        uv_read_stop((uv_stream_t*)&(tcp->uv_obj));
        tcp->reading = false;
    }
}

static void free_write_req(stream_write_req* wr)
{
    default_allocator.free(wr->buf.base);

    wr->~stream_write_req();
    default_allocator.free(wr);
}

static void stream_write_callback(uv_write_t* req, int status)
{
    auto* wr = (stream_write_req*)req;
    auto* tcp = (tcp_item*)(req->handle->data);
    if (status < 0) {
        wr->callback(fail, tcp->id);
    } else {
        wr->callback(ok, tcp->id);
    }
    free_write_req(wr);
}

void tcp_write(
    rid tcp_id, const void* buffer, size_t size, const tcp_write_callback& cb)
{

    auto* tcp = find_tcp_item(tcp_id, &tcps);

    auto* req = (stream_write_req*)default_allocator.allocate(
        sizeof(stream_write_req));
    new (req) stream_write_req;
    req->callback = cb;
    req->buf = uv_buf_init((char*)default_allocator.allocate(size), size);
    memcpy(req->buf.base, buffer, size);

    if (uv_write((uv_write_t*)req, (uv_stream_t*)&(tcp->uv_obj), &req->buf, 1,
            stream_write_callback)
        < 0) {
        free_write_req(req);

        create_idle([tcp_id, cb](auto self) {
            destroy_idle(self);
            cb(fail, tcp_id);
        });
    }
}

tcp_write_callback default_tcp_write_callback { [](auto status, auto id) {
    unused(status, id);
} };

bool tcp_alive(rid tcp_id) { return find_tcp_item(tcp_id, &tcps); }

} // namespace nx::aio