#ifndef NX_AIO_H
#define NX_AIO_H

#include <functional>

#include <niu2x/utils.h>
#include <niu2x/list_head.h>

namespace nx::aio {

enum API type {
    wait_all,
    wait_once,
    no_wait,
};

API void run(int type = wait_all);
API void set_allocator(memory_proxy allocator);

using rid = uint64_t;

using idle_handle = std::function<void(rid self)>;
API rid create_idle(const idle_handle& handler);
API void destroy_idle(rid idle);

using tcp_connect_callback = std::function<void(status, rid self)>;
API rid create_tcp();
API void destroy_tcp(rid tcp);
API void tcp_connect(
    rid tcp, const char* ip, uint16_t port, const tcp_connect_callback& cb);
API void tcp_bind(rid tcp, const char* ip, uint16_t port);

using tcp_read_callback
    = std::function<void(status, rid self, const uint8_t*, size_t size)>;
API void tcp_read_start(rid tcp, const tcp_read_callback& cb);
API void tcp_read_stop(rid tcp);

using tcp_write_callback = std::function<void(status, rid self)>;
API void tcp_write(
    rid tcp, const uint8_t* buffer, size_t size, const tcp_write_callback&);

// API void destroy(rid id);

} // namespace nx::aio

#endif