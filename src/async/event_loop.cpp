#include <niu2x/async/event_loop.h>
#include <niu2x/utils.h>

#include <niu2x/build.h>
#if defined(libuv_FOUND)

namespace nx::async {

event_loop_t::event_loop_t() { uv_loop_init(&loop_); }

event_loop_t::~event_loop_t()
{
    if (idles_.size() > 0) {
        NX_LOG_E("memory leak");
    }
    uv_loop_close(&loop_);
}

void event_loop_t::run() { uv_run(&loop_, UV_RUN_DEFAULT); }

event_loop_t::id_t event_loop_t::idle_start(idle_callback_t callback)
{
    auto idle = new idle_t;
    idle->id = id_alloc();
    idle->callback = std::move(callback);
    uv_idle_init(&loop_, &(idle->idle));
    idle->idle.data = idle;

    idles_[idle->id] = idle;

    uv_idle_start(&(idle->idle), uv_idle_cb);
    return idle->id;
}

void event_loop_t::idle_stop(id_t id)
{
    auto idle = idles_[id];
    idles_.erase(id);

    uv_idle_stop(&(idle->idle));
    id_free(idle->id);
    delete idle;
}

event_loop_t::id_t event_loop_t::id_alloc() { return 0; }
void event_loop_t::id_free(id_t) { }

void event_loop_t::uv_idle_cb(uv_idle_t* handle) { unused(handle); }

} // namespace nx::async
#endif