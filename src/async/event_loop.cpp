#include <niu2x/async/event_loop.h>
#include <niu2x/utils.h>

#include <niu2x/build.h>
#include <niu2x/bitmap.h>

#if defined(libuv_FOUND)
    #include <uv.h>

namespace nx::async {

static void uv_idle_cb(uv_idle_t* handle);

struct idle_t {
    uv_idle_t idle;
    event_loop_t::idle_callback_t callback;
    int id;
};

event_loop_t::event_loop_t()
{
    loop_ = (uv_loop_t*)malloc(sizeof(uv_loop_t));
    NX_THROW_COND_MSG(!loop_, "out of memory");
    uv_loop_init((uv_loop_t*)loop_);

    id_alloc_ = 64;
    id_nr_ = 0;
    id_bitmap_ = bitmap_zalloc(id_alloc_);
    NX_THROW_COND_MSG(!id_bitmap_, "out of memory");
}

event_loop_t::~event_loop_t()
{
    if (idles_.size() > 0) {
        NX_LOG_E("event_loop_t::~event_loop_t idles exist");
    }
    uv_loop_close((uv_loop_t*)loop_);
    free(loop_);
}

void event_loop_t::run() { uv_run((uv_loop_t*)loop_, UV_RUN_DEFAULT); }

event_loop_t::id_t event_loop_t::idle_start(idle_callback_t callback)
{
    auto idle = new (std::nothrow) idle_t;
    NX_FAIL_COND_V_MSG(!idle, INVALID_ID, "out of memory");

    idle->id = id_alloc();
    idle->callback = std::move(callback);
    uv_idle_init((uv_loop_t*)loop_, &(idle->idle));
    idle->idle.data = idle;
    idles_[idle->id] = idle;
    uv_idle_start(&(idle->idle), uv_idle_cb);
    return idle->id;
}

void event_loop_t::idle_stop(id_t id)
{
    auto idle = idles_[id];
    idles_.erase(id);
    uv_idle_stop(&(((idle_t*)idle)->idle));
    id_free(((idle_t*)idle)->id);
    delete (idle_t*)idle;
}

event_loop_t::id_t event_loop_t::id_alloc() { return 0; }
void event_loop_t::id_free(id_t) { }

static void uv_idle_cb(uv_idle_t* handle)
{
    auto idle = (idle_t*)(handle->data);
    idle->callback(idle->id);
}

} // namespace nx::async
#endif