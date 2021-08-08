#ifndef ASYNC_IO_H
#define ASYNC_IO_H

#include <uv.h>

#include <niu2x/utils.h>
#include <niu2x/freelist.h>

namespace nx::aio {

enum API run_type {
    wait_all,
    wait_once,
    no_wait,
};

class API event_loop : private noncopyable {
public:
    using idle_handle = std::function<void(rid self)>;

    using connect_handle = std::function<void(int, rid self)>;

    using read_handle = std::function<void(
        int, rid self, const uint8_t* buffer, size_t size)>;

    event_loop(const mm::allocator& allocator = default_allocator);
    ~event_loop();

    void run(run_type p_type);
    void run() { run(wait_all); }

    rid create_idle(idle_handle handle);
    void destroy_idle(rid);

    rid create_tcp();
    void destroy_tcp(rid tcp_id);

    void connect(
        rid tcp_id, const char* address, uint16_t port, connect_handle);

    void start_read(rid stream_id, read_handle callback);
    void stop_read(rid stream_id);

private:
    struct idle_t {
        uv_idle_t uv_obj;
        union {
            event_loop::idle_handle callback;
        };
    };

    struct stream_t {
        union {
            uv_tcp_t uv_tcp;
        };
        enum {
            tcp,
            udp,
        } type;

        event_loop::read_handle read_cb;
    };

    struct connect_t {
        uv_connect_t uv_obj;
        union {
            event_loop::connect_handle callback;
        };
        struct sockaddr_in dest;
    };

    uv_loop_t loop_;

    freelist<idle_t, 4> idles_;

    freelist<stream_t, 1024> streams_;

    freelist<connect_t, 1024> connects_;

    static void idle_callback(uv_idle_t* handle);
    static void connect_callback(uv_connect_t* uv_con, int status);
    static void my_alloc_cb(
        uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
    void free_buf(const uv_buf_t* buf);
    static void read_callback(
        uv_stream_t* uv_stream, ssize_t nread, const uv_buf_t* buf);

    rid create_connect(rid tcp_id, const char* address, uint16_t port,
        connect_handle callback);
    void destroy_connect(rid con_id);

    mm::allocator allocator_;
};

} // namespace nx::aio

#endif