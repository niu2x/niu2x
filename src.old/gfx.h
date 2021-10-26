#ifndef NX_GFX_H
#define NX_GFX_H

#include <functional>
#include <string>

#include <niu2x/utils.h>
#include <niu2x/freelist.h>
#include <niu2x/list_head.h>

namespace nx::gfx {

API void set_allocator(memory_proxy allocator);

using rid = void*;

enum API option {
    MSAA = 1 << 0,
};

const uint32_t default_option = MSAA;

struct API color {
    union {
        uint32_t rgba;
        struct {
            uint8_t r, g, b, a;
        };
    };
};

inline color make_color(uint32_t rgba8888)
{
    color ret;
    ret.rgba = rgba8888;
    return ret;
}

struct API config {
    std::string title;
    int width, height;
    uint64_t options;
    void (*update)(double);
    void (*setup)();
    void (*cleanup)();
};

API void run(const config& c);

enum API attribute {
    position,
    color0,
    uv,
};

class API vertex_layout_builder {
};
class API vertex_layout {
public:
    struct API attrinfo {
        attribute attr;
    };

private:
};

} // namespace nx::gfx

#endif