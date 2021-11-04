#ifndef NX_GFX_H
#define NX_GFX_H

#include <string>
#include <vector>

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

struct NXAPI object_t {
    uint8_t type;
    uint64_t id;
};

struct NXAPI texture_t : object_t {
    GLuint name;
};

enum class vertex_attr_type {
    position,
    color,
};

using vertex_layout_t = std::vector<vertex_attr_type>;

struct NXAPI vertex_buffer_t : object_t {
    GLuint name;
};

struct NXAPI indice_buffer_t : object_t {
    GLuint name;
};

struct NXAPI program_t : object_t {
    GLuint name;
};

NXAPI vertex_buffer_t* create_vertex_buffer();
NXAPI void destroy(object_t*);

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
NXAPI void set_vertex_buffer(vertex_buffer_t* vbo);

} // namespace nx::gfx

#endif