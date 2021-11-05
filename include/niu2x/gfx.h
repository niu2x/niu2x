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

enum class vertex_attr_type : uint8_t {
    nil = 0,
    position,
    color,
    normal,
    uv,
};

using vertex_layout_t = uint64_t;
NXAPI vertex_layout_t vertex_layout(vertex_attr_type a0 = vertex_attr_type::nil,
    vertex_attr_type a1 = vertex_attr_type::nil,
    vertex_attr_type a2 = vertex_attr_type::nil,
    vertex_attr_type a3 = vertex_attr_type::nil,
    vertex_attr_type a4 = vertex_attr_type::nil,
    vertex_attr_type a5 = vertex_attr_type::nil,
    vertex_attr_type a6 = vertex_attr_type::nil,
    vertex_attr_type a7 = vertex_attr_type::nil,
    vertex_attr_type a8 = vertex_attr_type::nil,
    vertex_attr_type a9 = vertex_attr_type::nil,
    vertex_attr_type a10 = vertex_attr_type::nil,
    vertex_attr_type a11 = vertex_attr_type::nil,
    vertex_attr_type a12 = vertex_attr_type::nil,
    vertex_attr_type a13 = vertex_attr_type::nil,
    vertex_attr_type a14 = vertex_attr_type::nil,
    vertex_attr_type a15 = vertex_attr_type::nil);

struct NXAPI vertex_buffer_t : object_t {
    GLuint name;
    vertex_layout_t layout;
};

struct NXAPI indice_buffer_t : object_t {
    GLuint name;
};

struct NXAPI program_t : object_t {
    GLuint name;
};

NXAPI vertex_buffer_t* create_vertex_buffer(
    vertex_layout_t layout, uint32_t vertices_count);
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