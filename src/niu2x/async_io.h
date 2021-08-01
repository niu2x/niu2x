#ifndef ASYNC_IO_H
#define ASYNC_IO_H

#include <niu2x/utils.h>
#include <niu2x/misc/freelist.h>

namespace nx::async_io {

class API event_loop: private noncopyable {
public:
    using rid = misc::rid;

    template <class T, misc::freelist_uint_t N>
    using freelist = misc::freelist<T, N>;

    enum type {
        wait,
        wait_once,
        no_wait,
    };

    using idle_handle = std::function<void(rid self)>;

    static std::unique_ptr<event_loop> create();

    event_loop();
    virtual ~event_loop();

    virtual void run(type p_type) = 0;
    void run() { run(type::wait); }

    virtual rid create_idle(idle_handle handle) = 0;
    virtual void destroy_idle(rid) = 0;

private:

};

// class idle: private noncopyable {
// public:
// 	using callback = std::function<void (idle *)>;
// 	idle(event_loop *p_loop, callback p_callback);
// 	virtual ~idle() = 0;
// private:
// 	callback callback_;
// 	event_loop
// };

} // namespace nx::async_io

namespace nx::aio {

using namespace nx::async_io;

} // namespace nx::aio

#endif