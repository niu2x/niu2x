#ifndef NX_GFX_H
#define NX_GFX_H

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <niu2x/api.h>

namespace nx::gfx {

enum NXAPI window_option {
    MSAA = 1 << 0,
};

struct NXAPI window_config {
    std::string title;
    int width, height;
    uint64_t options;
    void (*update)(double);
    void (*setup)();
    void (*cleanup)();
};

NXAPI void run(const window_config& c);

struct NXAPI texture_t {
    GLuint name;
};

struct NXAPI vertex_buffer_t {
    GLuint name;
};

// NXAPI vertex_buffer_t create_vertex_buffer

// NXAPI void kick();

struct NXAPI rgba8888_t {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

union NXAPI color_t {
    struct rgba8888_t rgba;
    uint32_t u;
};

inline color_t rgba(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
{
    return color_t { .u = (r) | (g << 8) | (b << 16) | (a << 24) };
}

using layer_t = uint8_t;

NXAPI void begin();
NXAPI void end();

NXAPI void clear(layer_t layer);
NXAPI void set_clear_color(color_t color);

} // namespace nx::gfx

#endif