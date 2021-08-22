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

} // namespace nx::aio

#endif