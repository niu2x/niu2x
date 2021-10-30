#ifndef NX_GFX_H
#define NX_GFX_H

#include <string>

#include <niu2x/api.h>

namespace nx::gfx {

enum API window_option {
    MSAA = 1 << 0,
};

struct API window_config {
    std::string title;
    int width, height;
    uint64_t options;
    void (*update)(double);
    void (*setup)();
    void (*cleanup)();
};

API void run(const window_config& c);

} // namespace nx::gfx

#endif