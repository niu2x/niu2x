#include "async_io.h"

#include <uv.h>
#include <niu2x/misc/freelist.h>
#include <niu2x/misc/net_utils.h>
#include <niu2x/log.h>

namespace nx::async_io {

namespace {

struct idle {
    uv_idle_t uv_obj;
    union {
        event_loop::idle_handle callback;
    };
};

struct tcp {
    uv_tcp_t uv_obj;
};

struct connect_t {
    uv_connect_t uv_obj;
    union {
        event_loop::connect_handle callback;
    };
    struct sockaddr_in dest;
};

class uv_event_loop : public event_loop, private noncopyable {
public:
    uv_event_loop()
    {
        NX_LOG_D("uv_event_loop::uv_event_loop");
        uv_loop_init(&loop_);
        loop_.data = this;
    }
    virtual ~uv_event_loop()
    {
        uv_loop_close(&loop_);
        NX_LOG_D("uv_event_loop::~uv_event_loop");
    }

    virtual void run(type p_type) override
    {
        switch (p_type) {
            case type::wait: {
                uv_run(&loop_, UV_RUN_DEFAULT);
                break;
            }
            case type::wait_once: {
                uv_run(&loop_, UV_RUN_ONCE);
                break;
            }
            case type::no_wait: {
                uv_run(&loop_, UV_RUN_NOWAIT);
                break;
            }
        }
    }

    virtual rid create_idle(idle_handle handle) override
    {
        rid idle_id = idles_.alloc();
        if (idle_id != rid::nil) {
            auto* idle = idles_.get(idle_id);
            uv_idle_init(&loop_, &(idle->uv_obj));
            idle->uv_obj.data = reinterpret_cast<void*>(idle_id.id);
            uv_idle_start(&(idle->uv_obj), idle_callback);
            new (&(idle->callback)) event_loop::idle_handle(handle);
        }

        return idle_id;
    }
    virtual void destroy_idle(rid idle_id) override
    {
        if (idle_id == rid::nil)
            return;
        auto* idle = idles_.get(idle_id);
        NX_ASSERT(idle, "invalid idle ptr");
        destructor<event_loop::idle_handle>::destory(&(idle->callback));
        uv_idle_stop(&(idle->uv_obj));
        uv_close(reinterpret_cast<uv_handle_t*>(&(idle->uv_obj)), nullptr);
        idles_.free(idle_id);
    }

    virtual rid create_tcp() override
    {
        rid tcp_id = tcps_.alloc();
        if (tcp_id != rid::nil) {
            auto* tcp = tcps_.get(tcp_id);
            uv_tcp_init(&loop_, &(tcp->uv_obj));
        }
        return tcp_id;
    }

    virtual void destory_tcp(rid tcp_id) override
    {
        if (tcp_id == rid::nil)
            return;
        auto* tcp = tcps_.get(tcp_id);
        NX_ASSERT(tcp, "invalid tcp ptr");
        uv_close(reinterpret_cast<uv_handle_t*>(&(tcp->uv_obj)), nullptr);
        tcps_.free(tcp_id);
    }

    virtual void connect(rid tcp_id, const char* address, uint16_t port,
        connect_handle callback) override
    {
        if (create_connect(tcp_id, address, port, callback) == rid::nil) {
            // todo delay callback
            NX_LOG_E("tcp failed to connect: %s %d", address, port);
            callback(status::fail, tcp_id);
        }
    }

private:
	uv_loop_t loop_;

    freelist<idle, 4> idles_;
    static void idle_callback(uv_idle_t* handle)
    {
        rid idle_id { rid::id_t(uint64_t(handle->data)) };
        NX_ASSERT(idle_id != rid::nil, "can't find this handle");
        uv_loop_t* uv_loop = handle->loop;
        auto* my_loop = reinterpret_cast<uv_event_loop*>(uv_loop->data);
        auto* idle = my_loop->idles_.get(idle_id);
        idle->callback(idle_id);
    }

    freelist<tcp, 1024> tcps_;
    misc::freelist<connect_t, 1024> connects_;

    rid create_connect(
        rid tcp_id, const char* address, uint16_t port, connect_handle callback)
    {

        rid con_id = connects_.alloc();
        if (con_id != rid::nil) {
            auto* con = connects_.get(con_id);
            con->uv_obj.data = reinterpret_cast<void*>(con_id.id);
            new (&(con->callback)) event_loop::connect_handle(callback);
            struct sockaddr_in* p_dest = &(con->dest);
            uv_ip4_addr(address, port, p_dest);
            uv_tcp_connect(&(con->uv_obj), &(tcps_.get(tcp_id)->uv_obj),
                (const struct sockaddr*)p_dest, connect_callback);
        }

        return con_id;
    }

    void destroy_connect(rid con_id)
    {
        if (con_id == rid::nil)
            return;
        auto* con = connects_.get(con_id);
        NX_ASSERT(con, "invalid idle ptr");
        destructor<event_loop::connect_handle>::destory(&(con->callback));
        tcps_.free(con_id);
    }

    static void connect_callback(uv_connect_t* uv_con, int status)
    {

        uv_loop_t* uv_loop = uv_con->handle->loop;
        auto* my_loop = reinterpret_cast<uv_event_loop*>(uv_loop->data);

        rid con_id { rid::id_t(uint64_t(uv_con->data)) };

        uv_tcp_t* uv_tcp = reinterpret_cast<uv_tcp_t*>(uv_con->handle);
        rid tcp_id { rid::id_t(uint64_t(uv_tcp->data)) };

        auto* con = my_loop->connects_.get(con_id);

        if (status < 0) {
            NX_LOG_E("tcp failed to connect: %s reason: %s",
                misc::net_utils::desc((struct sockaddr*)&(con->dest)).c_str(),
                uv_strerror(status));
            con->callback(aio::status::fail, tcp_id);
        } else {
            con->callback(aio::status::ok, tcp_id);
        }

        my_loop->destroy_connect(con_id);
    }
};

NX_INIT_FUNC(init_uv_allocator) { }

} // namespace

} // namespace nx::async_io

namespace nx::async_io {


event_loop::event_loop() {
}
event_loop::~event_loop() {
}

std::unique_ptr<event_loop> event_loop::create() {
    std::cout << "uv_event_loop size: " << sizeof(uv_event_loop) << std::endl;
    return std::make_unique<uv_event_loop>();
}

} // namespace nx::async_io
