#ifndef NX_ASYNC_EVENT_LOOP_H
#define NX_ASYNC_EVENT_LOOP_H

#include <vector>
#include <map>
#include <functional>
#include <boost/noncopyable.hpp>
#include <niu2x/api.h>
#include <uv.h>

namespace nx::async {

class event_loop_t : private boost::noncopyable {
public:
    using id_t = int;
    using idle_callback_t = std::function<void(id_t)>;
    struct idle_t {
        uv_idle_t idle;
        idle_callback_t callback;
        int id;
    };

    event_loop_t();
    ~event_loop_t();

    void run();

    id_t idle_start(idle_callback_t callback);
    void idle_stop(id_t);

private:
    id_t id_alloc();
    void id_free(id_t);

    uv_loop_t loop_;
    std::map<id_t, idle_t*> idles_;

    static void uv_idle_cb(uv_idle_t* handle);
};

} // namespace nx::async

#endif