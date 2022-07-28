#ifndef NX_ASYNC_EVENT_LOOP_H
#define NX_ASYNC_EVENT_LOOP_H

#include <niu2x/build.h>
#if defined(libuv_FOUND)

    #include <vector>
    #include <map>
    #include <functional>
    #include <boost/noncopyable.hpp>
    #include <niu2x/api.h>
    #include <niu2x/bitmap.h>

namespace nx::async {

class NXAPI event_loop_t : private boost::noncopyable {
public:
    using id_t = int;
    enum { INVALID_ID = -1 };
    using idle_callback_t = std::function<void(id_t)>;

    event_loop_t();
    ~event_loop_t();

    void run();

    id_t idle_start(idle_callback_t callback);
    void idle_stop(id_t);

private:
    id_t id_alloc();
    void id_free(id_t);

    void* loop_;
    std::map<id_t, void*> idles_;

    bitmap_t id_bitmap_;
    int id_alloc_;
    int id_nr_;

    // static void uv_idle_cb(uv_idle_t* handle);
};

} // namespace nx::async

#endif
#endif