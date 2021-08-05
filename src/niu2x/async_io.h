#ifndef ASYNC_IO_H
#define ASYNC_IO_H

#include <niu2x/utils.h>
#include <niu2x/misc/freelist.h>

namespace nx::async_io {

enum class API status {
    ok = 0,
    fail,
};

class API event_loop {
public:
    using rid = misc::rid;

    template <class T, misc::freelist_uint_t N>
    using freelist = misc::freelist<T, N>;

    enum type {
        wait = 0,
        wait_once,
        no_wait,
    };

    using idle_handle = std::function<void(rid self)>;
    using connect_handle = std::function<void(status, rid self)>;
    using read_handle = std::function<void(
        status, rid self, const uint8_t* buffer, size_t size)>;

    static std::unique_ptr<event_loop> create();

    event_loop();
    virtual ~event_loop();
    event_loop(const event_loop&) = default;
    event_loop& operator=(const event_loop&) = default;

    virtual void run(type p_type) = 0;
    void run() { run(type::wait); }

    virtual rid create_idle(idle_handle handle) = 0;
    virtual void destroy_idle(rid) = 0;

    virtual rid create_tcp() = 0;
    virtual void destory_tcp(rid tcp_id) = 0;

    virtual void connect(
        rid tcp_id, const char* address, uint16_t port, connect_handle)
        = 0;

    virtual void start_read(rid stream_id, read_handle callback) = 0;
    virtual void stop_read(rid stream_id) = 0;
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