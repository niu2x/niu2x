#include "aio.h"

#include <niu2x/misc/net_utils.h>
#include <niu2x/log.h>

namespace nx::aio {

event_loop::event_loop(const mm::allocator& allocator)
: allocator_(allocator)
{
    NX_LOG_T("event_loop::event_loop");
    uv_loop_init(&loop_);
    loop_.data = this;
}

event_loop::~event_loop()
{
    uv_loop_close(&loop_);
    NX_LOG_T("event_loop::~event_loop");
}

void event_loop::run(run_type p_type)
{
    switch (p_type) {
        case wait_all: {
            uv_run(&loop_, UV_RUN_DEFAULT);
            break;
        }
        case wait_once: {
            uv_run(&loop_, UV_RUN_ONCE);
            break;
        }
        case no_wait: {
            uv_run(&loop_, UV_RUN_NOWAIT);
            break;
        }
    }
}

rid event_loop::create_idle(idle_handle handle)
{
    rid idle_id = idles_.alloc();
    if (idle_id != nil) {
        auto* idle = idles_.get(idle_id);
        uv_idle_init(&loop_, &(idle->uv_obj));
        idle->uv_obj.data = reinterpret_cast<void*>(idle_id);
        uv_idle_start(&(idle->uv_obj), idle_callback);
        new (&(idle->callback)) event_loop::idle_handle(handle);
    }
    return idle_id;
}

void event_loop::idle_callback(uv_idle_t* handle)
{
    rid idle_id = reinterpret_cast<rid>(handle->data);
    NX_ASSERT(idle_id != nil, "can't find this handle");
    uv_loop_t* uv_loop = handle->loop;
    auto* my_loop = reinterpret_cast<event_loop*>(uv_loop->data);
    auto* idle = my_loop->idles_.get(idle_id);
    idle->callback(idle_id);
}

void event_loop::connect_callback(uv_connect_t* uv_con, int status)
{

    uv_loop_t* uv_loop = uv_con->handle->loop;
    auto* my_loop = reinterpret_cast<event_loop*>(uv_loop->data);

    rid con_id = reinterpret_cast<rid>(uv_con->data);

    uv_tcp_t* uv_tcp = reinterpret_cast<uv_tcp_t*>(uv_con->handle);
    rid tcp_id = reinterpret_cast<rid>(uv_tcp->data);

    auto* con = my_loop->connects_.get(con_id);

    if (status < 0) {

        struct sockaddr_in* addr_in = (struct sockaddr_in*)(&(con->dest));
        char* s = inet_ntoa(addr_in->sin_addr);

        NX_LOG_E(
            "tcp failed to connect: %s reason: %s", s, uv_strerror(status));
        con->callback(fail, tcp_id);
    } else {
        con->callback(ok, tcp_id);
    }

    my_loop->destroy_connect(con_id);
}

void event_loop::my_alloc_cb(
    uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
    uv_loop_t* uv_loop = handle->loop;
    auto* my_loop = reinterpret_cast<event_loop*>(uv_loop->data);

    buf->base = (char*)my_loop->allocator_.allocate(suggested_size);
    buf->len = (ULONG)(buf->base ? suggested_size : 0);
}

void event_loop::free_buf(const uv_buf_t* buf)
{
    if (buf->base && buf->len) {
        allocator_.free(buf->base);
    }
}

void event_loop::read_callback(
    uv_stream_t* uv_stream, ssize_t nread, const uv_buf_t* buf)
{

    uv_loop_t* uv_loop = uv_stream->loop;
    auto* my_loop = reinterpret_cast<event_loop*>(uv_loop->data);
    rid stream_id = reinterpret_cast<rid>(uv_stream->data);
    auto* stream = my_loop->streams_.get(stream_id);

    unused(stream, stream_id);

    if (nread < 0) {
        stream->read_cb(fail, stream_id, nullptr, 0);
    }

    else if (nread > 0) {
        if (stream->read_cb)
            stream->read_cb(ok, stream_id, (const uint8_t*)buf->base, nread);
    }

    my_loop->free_buf(buf);
}

void event_loop::destroy_connect(rid con_id)
{
    if (con_id == nil)
        return;
    auto* con = connects_.get(con_id);
    NX_ASSERT(con, "invalid idle ptr");
    destructor<event_loop::connect_handle>::destroy(&(con->callback));
    connects_.free(con_id);
}

rid event_loop::create_connect(
    rid tcp_id, const char* address, uint16_t port, connect_handle callback)
{
    rid con_id = connects_.alloc();
    if (con_id != nil) {
        auto* con = connects_.get(con_id);
        con->uv_obj.data = reinterpret_cast<void*>(con_id);
        new (&(con->callback)) event_loop::connect_handle(callback);

        struct sockaddr_in* p_dest = &(con->dest);
        uv_ip4_addr(address, port, p_dest);
        uv_tcp_connect(&(con->uv_obj), &(streams_.get(tcp_id)->uv_tcp),
            (const struct sockaddr*)p_dest, connect_callback);
    }
    return con_id;
}

void event_loop::destroy_idle(rid idle_id)
{
    if (idle_id == nil)
        return;
    auto* idle = idles_.get(idle_id);
    NX_ASSERT(idle, "invalid idle ptr");
    destructor<event_loop::idle_handle>::destroy(&(idle->callback));
    uv_idle_stop(&(idle->uv_obj));
    uv_close(reinterpret_cast<uv_handle_t*>(&(idle->uv_obj)), nullptr);
    idles_.free(idle_id);
}

rid event_loop ::create_tcp()
{
    rid tcp_id = streams_.alloc();
    if (tcp_id != nil) {
        auto* tcp = streams_.get(tcp_id);
        tcp->type = stream_t::tcp;
        new (&(tcp->read_cb)) event_loop::read_handle();
        uv_tcp_init(&loop_, &(tcp->uv_tcp));
        tcp->uv_tcp.data = reinterpret_cast<void*>(tcp_id);
    }

    NX_LOG_T("create_tcp %u", tcp_id);
    return tcp_id;
}

void event_loop::destroy_tcp(rid tcp_id)
{
    NX_LOG_T("destroy_tcp %u", tcp_id);
    if (tcp_id == nil)
        return;
    auto* tcp = streams_.get(tcp_id);
    NX_ASSERT(tcp, "invalid tcp ptr");
    uv_close(reinterpret_cast<uv_handle_t*>(&(tcp->uv_tcp)), nullptr);
    destructor<event_loop::read_handle>::destroy(&(tcp->read_cb));
    streams_.free(tcp_id);
}

void event_loop::connect(
    rid tcp_id, const char* address, uint16_t port, connect_handle callback)
{
    if (create_connect(tcp_id, address, port, callback) == nil) {
        // todo delay callback
        NX_LOG_E("tcp failed to connect: %s %d", address, port);
        callback(fail, tcp_id);
    }
}

void event_loop::start_read(rid stream_id, read_handle callback)
{
    if (stream_id != nil) {
        auto* my_stream = streams_.get(stream_id);
        if (my_stream->read_cb != nullptr) {
            NX_LOG_W("steam is already start read");
            return;
        }

        my_stream->read_cb = callback;
        switch (my_stream->type) {
            case stream_t::tcp: {
                auto s = uv_read_start((uv_stream_t*)&(my_stream->uv_tcp),
                    my_alloc_cb, read_callback);
                if (s) {
                    NX_LOG_E("tcp failed to start_read: %d, reason: %s", s,
                        uv_strerror(s));
                }
                break;
            }
            default: {
            }
        }
    }
}

void event_loop::stop_read(rid stream_id)
{
    if (stream_id != nil) {
        auto* my_stream = streams_.get(stream_id);
        switch (my_stream->type) {
            case stream_t::tcp: {
                uv_read_stop((uv_stream_t*)&(my_stream->uv_tcp));
                break;
            }
            default: {
            }
        }
    }
}

} // namespace nx::aio