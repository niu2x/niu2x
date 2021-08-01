#include "async_io.h"

#include <uv.h>
#include <niu2x/misc/freelist.h>

namespace nx::async_io {

namespace {

struct idle {
    uv_idle_t uv_obj;
    union {
        event_loop::idle_handle callback;
    };
};

class uv_event_loop: public event_loop {
public:
    uv_event_loop()
    {
        uv_loop_init(&loop_);
        loop_.data = this;
    }
    virtual ~uv_event_loop() { uv_loop_close(&loop_); }

    virtual void run(type p_type)
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

    virtual rid create_idle(idle_handle handle)
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
    virtual void destroy_idle(rid idle_id)
    {
        if (idle_id == rid::nil)
            return;
        auto* idle = idles_.get(idle_id);
        NX_ASSERT(idle, "invalid idle ptr");
        destructor<event_loop::idle_handle>::destory(&(idle->callback));
        uv_idle_stop(&(idle->uv_obj));
        idles_.free(idle_id);
    }

private:
	uv_loop_t loop_;
    freelist<idle, 64> idles_;
    static void idle_callback(uv_idle_t* handle)
    {
        rid idle_id { rid::id_t(uint64_t(handle->data)) };
        NX_ASSERT(idle_id != rid::nil, "can't find this handle");
        uv_loop_t* uv_loop = handle->loop;
        auto* my_loop = reinterpret_cast<uv_event_loop*>(uv_loop->data);
        auto* idle = my_loop->idles_.get(idle_id);
        idle->callback(idle_id);
    }
};

} // namespace

} // namespace nx::async_io

namespace nx::async_io {


event_loop::event_loop() {
}
event_loop::~event_loop() {
}

std::unique_ptr<event_loop> event_loop::create() {
	return std::make_unique<uv_event_loop>();
}

} // namespace nx::async_io
