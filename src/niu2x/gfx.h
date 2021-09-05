#ifndef NX_GFX_H
#define NX_GFX_H

#include <functional>

#include <niu2x/utils.h>
#include <niu2x/freelist.h>
#include <niu2x/list_head.h>

namespace nx::gfx {

API void set_allocator(memory_proxy allocator);

using rid = void*;

enum window_option {
    MSAA = 1 << 0,
};

API rid create_window(uint64_t options = 0);
API void destroy_window(rid window_id);
// rgba8888
API void window_set_bgcolor(rid window_id, uint32_t color);

API void frame();

} // namespace nx::gfx

#endif