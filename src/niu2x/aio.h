// #ifndef ASYNC_IO_H
// #define ASYNC_IO_H

// #include <niu2x/utils.h>
// #include <niu2x/misc/freelist.h>

// namespace nx::aio {

// enum API status {
//     ok = 0,
//     fail,
// };

// enum API wait_type {
//     wait_all = 0,
//     wait_once,
//     no_wait,
// };

// class API event_loop {
// public:
//     using rid = misc::rid;

//     template <class T, misc::freelist_uint_t N>
//     using freelist = misc::freelist<T, N>;

//     using idle_handle = std::function<void(rid self)>;
//     using connect_handle = std::function<void(int, rid self)>;
//     using read_handle = std::function<void(
//         int, rid self, const uint8_t* buffer, size_t size)>;

//     static std::unique_ptr<event_loop> create();

//     event_loop();
//     virtual ~event_loop();
//     event_loop(const event_loop&) = default;
//     event_loop& operator=(const event_loop&) = default;

//     virtual void run(int p_type) = 0;
//     void run() { run(wait_all); }

//     virtual rid create_idle(idle_handle handle) = 0;
//     virtual void destroy_idle(rid) = 0;

//     virtual rid create_tcp() = 0;
//     virtual void destory_tcp(rid tcp_id) = 0;

//     virtual void connect(
//         rid tcp_id, const char* address, uint16_t port, connect_handle)
//         = 0;

//     virtual void start_read(rid stream_id, read_handle callback) = 0;
//     virtual void stop_read(rid stream_id) = 0;
// };

// } // namespace nx::aio

// namespace nx::async_io {

// using namespace nx::aio;

// } // namespace nx::async_io

// #endif