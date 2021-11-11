#ifndef NX_GFX_H
#define NX_GFX_H

#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <niu2x/api.h>
#include <niu2x/linmath.h>

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
    int width;
    int height;
};

enum class NXAPI pixel_format {
    rgba8,
    rgb8,
};

enum class vertex_attr_type : uint8_t {
    nil = 0,
    // vec3
    position,
    // vec4
    color,
    // vec3
    normal,
    // vec3
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

struct NXAPI limits {
    enum {
        max_uniform_name = 32,
        max_uniform = 32,
    };
};

struct NXAPI program_uniform_info_t {
    char name[limits::max_uniform_name];
    GLint location;
    GLenum type;
    GLint size;
};

struct NXAPI program_t : object_t {
    struct program_uniform_info_t uniforms[limits::max_uniform];
    int uniforms_size;
    GLuint name;
};

NXAPI vertex_buffer_t* create_vertex_buffer(
    vertex_layout_t layout, uint32_t vertices_count, void* data = nullptr);

NXAPI indice_buffer_t* create_indice_buffer(
    uint32_t indices_count, void* data = nullptr);

NXAPI program_t* create_program(const char* vert, const char* frag);

NXAPI texture_t* create_texture_2d(
    int w, int h, pixel_format pf, const void* data);
NXAPI texture_t* create_texture_2d_from_file(const char* pathname);

NXAPI void destroy(object_t*);

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

enum NXAPI render_state_constant {
    CULL_FRONT = 1 << 0,
    CULL_BACK = 1 << 1,
    WRITE_R = 1 << 2,
    WRITE_G = 1 << 3,
    WRITE_B = 1 << 4,
    WRITE_A = 1 << 5,
    WRITE_RGBA = WRITE_R | WRITE_G | WRITE_B | WRITE_A,
    WRITE_DEPTH = 1 << 6,
    WRITE_STENCIL = 1 << 7,
    DEPTH_TEST = 1 << 8,
    STENCIL_TEST = 1 << 9,
};

using render_state_t = uint32_t;
using texture_id_t = uint8_t;

NXAPI void begin();
NXAPI void end();

NXAPI void clear(layer_t layer);
NXAPI void draw_array(layer_t layer, uint32_t start, uint32_t count);
NXAPI void draw_element(layer_t layer, uint32_t start, uint32_t count);

NXAPI void set_clear_color(color_t color);
NXAPI void set_vertex_buffer(vertex_buffer_t* vbo);
NXAPI void set_indice_buffer(indice_buffer_t* vbo);
NXAPI void set_program(program_t* program);
NXAPI void set_render_state(render_state_t rs);
NXAPI void set_texture(texture_id_t tex_id, texture_t* tex);
NXAPI void reset();

// using mat4x4_element_t = float;
// struct NXAPI mat4x4_t {
//     mat4x4_element_t data[16];
// };

// NXAPI void mat4x4_set(struct mat4x4_t* output, const mat4x4_element_t*);
// NXAPI void mat4x4_mul(struct mat4x4_t* output, const struct mat4x4_t& a,
//     const struct mat4x4_t& b);
// NXAPI void mat4x4_add(struct mat4x4_t* output, const struct mat4x4_t& a,
//     const struct mat4x4_t& b);
// NXAPI void mat4x4_translate(struct mat4x4_t* output, float x, float y, float
// z); NXAPI void mat4x4_rotate_x(struct mat4x4_t* output, float r); NXAPI void
// mat4x4_rotate_y(struct mat4x4_t* output, float r); NXAPI void
// mat4x4_rotate_z(struct mat4x4_t* output, float r);

// NXAPI void mat4x4_lookat(struct mat4x4_t* output, float eyex, float eyey,
// float eyez, float lookx, float looky, float lookz, float upx, float upy,
// float upz); NXAPI void mat4x4_projection(struct mat4x4_t* output);

NXAPI void mat4x4_dump(const mat4x4 a);
// extern NXAPI struct mat4x4_t mat4x4_identity;

NXAPI void set_model_transform(const mat4x4);
NXAPI void set_view_transform(const mat4x4);
NXAPI void set_projection_transform(const mat4x4);

} // namespace nx::gfx

#endif