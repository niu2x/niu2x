#include <niu2x/aio.h>

#include <uv.h>

#define LOOP uv_default_loop()

namespace nx::aio {

namespace {

mallocfree_memory default_memory;
memory_proxy default_allocator(&default_memory);

rid next_rid = 0;

struct idle_item {
    rid id;
    uv_idle_t uv_obj;
    idle_handle callback;
    list_head list;
};

struct tcp_connect_item;
struct tcp_item {
    rid id;
    uv_tcp_t uv_obj;
    list_head list;
    tcp_connect_item* con;
    bool ready;
};

struct tcp_connect_item {
    rid id;
    tcp_item* tcp;
    list_head list;
    uv_connect_t uv_obj;
    struct sockaddr_in dest;
    tcp_connect_callback callback;
};

NX_LIST_HEAD(idles);
NX_LIST_HEAD(tcps);
NX_LIST_HEAD(tcp_connects);

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
    default_allocator.free(idle);

    NX_LOG_D("aio destroy_idle");
}

rid create_tcp()
{
    NX_LOG_D("aio create_tcp");
    auto* tcp = (tcp_item*)default_allocator.allocate(sizeof(tcp_item));
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
    default_allocator.free(tcp);
    NX_LOG_D("aio destroy_tcp");
}

static void destroy_tcp_connect(rid con_id)
{
    auto* con = find_tcp_connect_item(con_id, &tcp_connects);
    NX_ASSERT(con, "con_id isn't exist");
    list_del(&(con->list));
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

static rid create_tcp_connect(tcp_item* tcp, const char* ip, uint16_t port,
    const tcp_connect_callback& cb)
{
    NX_LOG_D("aio create_tcp_connect %s %u", ip, port);
    NX_ASSERT(tcp->con == nullptr && tcp->ready == false, "");
    auto* con = (tcp_connect_item*)default_allocator.allocate(
        sizeof(tcp_connect_item));
    con->id = next_rid++;
    list_add_tail(&(con->list), &tcp_connects);
    con->uv_obj.data = con;
    tcp->con = con;
    con->tcp = tcp;
    con->callback = cb;

    auto* addr = &(con->dest);

    uv_ip4_addr(ip, port, addr);
    uv_tcp_connect(&(con->uv_obj), &(tcp->uv_obj),
        (const struct sockaddr*)(addr), uv_tcp_connect_callback);

    return con->id;
}

API void tcp_connect(
    rid tcp_id, const char* ip, uint16_t port, const tcp_connect_callback& cb)
{
    auto* tcp = find_tcp_item(tcp_id, &tcps);
    NX_ASSERT(tcp, "tcp_id isn't exist");
    create_tcp_connect(tcp, ip, port, cb);
}

} // namespace nx::aio