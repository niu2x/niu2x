#ifndef NX_GFX_H
#define NX_GFX_H

#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <niu2x/api.h>
#include <niu2x/memory.h>
#include <niu2x/linmath.h>
#include <niu2x/list_head.h>

namespace nx::gfx {

enum NXAPI window_option_constant {
    MSAA = 1 << 0,
};

struct NXAPI window_config {
    std::string title;
    int width, height;
    uint64_t options;
    void (*update)(double);
    void (*setup)();
    void (*cleanup)();
    void (*key_callback)(int keycode, int action, int mods);
    void (*mouse_pos_callback)(double xpos, double ypos);
};

NXAPI void run(const window_config& c);
NXAPI void exit();

struct NXAPI object_t {
    uint8_t type;
    uint64_t id;
    list_head list;
};

enum class NXAPI pixel_format {
    rgba8,
    rgb8,
    r8,
};

struct NXAPI texture_t : object_t {
    GLuint name;
    int width;
    int height;
    pixel_format pf;
};

struct NXAPI font_t : object_t {
    void* private_data;
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
NXAPI vertex_layout_t vertex_layout_build(
    vertex_attr_type a0 = vertex_attr_type::nil,
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
    uint32_t size;
};

struct NXAPI indice_buffer_t : object_t {
    GLuint name;
    uint32_t size;
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

struct NXAPI font_char_info_t {
    texture_t* texture;
    float uv_x;
    float uv_y;
    float uv_w;
    float uv_h;
    int x, y, w, h;
    int advance;
};

struct mesh_t : object_t {
    vertex_buffer_t* vb;
    indice_buffer_t* ib;
};

NXAPI font_t* create_builtin_font(int font_size);
NXAPI font_char_info_t font_char_info(font_t* self, uint32_t code);
NXAPI int font_kerning(font_t* self, uint32_t left, uint32_t right);

NXAPI vertex_buffer_t* create_vertex_buffer(vertex_layout_t layout,
    uint32_t vertices_count, void* data = nullptr, bool auto_destroy = false);

NXAPI indice_buffer_t* create_indice_buffer(
    uint32_t indices_count, void* data = nullptr, bool auto_destroy = false);

NXAPI program_t* create_program(const char* vert, const char* frag);

NXAPI texture_t* create_texture_2d(
    int w, int h, pixel_format pf, const void* data);

NXAPI void texture_2d_update_region(
    texture_t* self, int x, int y, int w, int h, const void* data);

NXAPI texture_t* create_texture_2d_from_file(const char* pathname);

NXAPI mesh_t* create_mesh_from_file(const char* path, int idx = 0);

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
    BLEND = 1 << 10,
};

using blend_t = uint8_t;
struct NXAPI blend {
    enum {
        src_alpha,
        one_minus_src_alpha,
        one,
        zero,
        src_color,
        one_minus_src_color,
        dst_color,
        one_minus_dst_color,
        dst_alpha,
        one_minus_dst_alpha,
        // constant_color,
        // one_minus_dst_constant_color,
        // constant_alpha,
        // one_minus_dst_constant_alpha,
    };
};

using render_state_t = uint32_t;
using texture_id_t = uint8_t;

NXAPI void begin();
NXAPI void end();

NXAPI void set_clear_color(color_t color);

NXAPI void clear(layer_t layer);
NXAPI void draw_array(layer_t layer, uint32_t start, uint32_t count);
NXAPI void draw_element(layer_t layer, uint32_t start, uint32_t count);

NXAPI void set_vertex_buffer(vertex_buffer_t* vbo);
NXAPI void set_indice_buffer(indice_buffer_t* vbo);
NXAPI void set_program(program_t* program);
NXAPI void set_render_state(render_state_t rs);
NXAPI void set_texture(texture_id_t tex_id, texture_t* tex);
NXAPI void set_blend_func(blend_t src_func, blend_t dst_func);
NXAPI void set_model_transform(const mat4x4);
NXAPI void set_view_transform(const mat4x4);
NXAPI void set_projection_transform(const mat4x4);

NXAPI void reset();

NXAPI void mat4x4_dump(const mat4x4 a);

NXAPI void printf(int x, int y, const char*, ...);

enum NXAPI keycode_constant {
    KEY_UNKNOWN = -1,
    KEY_SPACE = 32,
    KEY_APOSTROPHE = 39,
    KEY_COMMA = 44,
    KEY_MINUS = 45,
    KEY_PERIOD = 46,
    KEY_SLASH = 47,
    KEY_0 = 48,
    KEY_1 = 49,
    KEY_2 = 50,
    KEY_3 = 51,
    KEY_4 = 52,
    KEY_5 = 53,
    KEY_6 = 54,
    KEY_7 = 55,
    KEY_8 = 56,
    KEY_9 = 57,
    KEY_SEMICOLON = 59,
    KEY_EQUAL = 61,
    KEY_A = 65,
    KEY_B = 66,
    KEY_C = 67,
    KEY_D = 68,
    KEY_E = 69,
    KEY_F = 70,
    KEY_G = 71,
    KEY_H = 72,
    KEY_I = 73,
    KEY_J = 74,
    KEY_K = 75,
    KEY_L = 76,
    KEY_M = 77,
    KEY_N = 78,
    KEY_O = 79,
    KEY_P = 80,
    KEY_Q = 81,
    KEY_R = 82,
    KEY_S = 83,
    KEY_T = 84,
    KEY_U = 85,
    KEY_V = 86,
    KEY_W = 87,
    KEY_X = 88,
    KEY_Y = 89,
    KEY_Z = 90,
    KEY_LEFT_BRACKET = 91,
    KEY_BACKSLASH = 92,
    KEY_RIGHT_BRACKET = 93,
    KEY_GRAVE_ACCENT = 96,
    KEY_WORLD_1 = 161,
    KEY_WORLD_2 = 162,
    KEY_ESCAPE = 256,
    KEY_ENTER = 257,
    KEY_TAB = 258,
    KEY_BACKSPACE = 259,
    KEY_INSERT = 260,
    KEY_DELETE = 261,
    KEY_RIGHT = 262,
    KEY_LEFT = 263,
    KEY_DOWN = 264,
    KEY_UP = 265,
    KEY_PAGE_UP = 266,
    KEY_PAGE_DOWN = 267,
    KEY_HOME = 268,
    KEY_END = 269,
    KEY_CAPS_LOCK = 280,
    KEY_SCROLL_LOCK = 281,
    KEY_NUM_LOCK = 282,
    KEY_PRINT_SCREEN = 283,
    KEY_PAUSE = 284,
    KEY_F1 = 290,
    KEY_F2 = 291,
    KEY_F3 = 292,
    KEY_F4 = 293,
    KEY_F5 = 294,
    KEY_F6 = 295,
    KEY_F7 = 296,
    KEY_F8 = 297,
    KEY_F9 = 298,
    KEY_F10 = 299,
    KEY_F11 = 300,
    KEY_F12 = 301,
    KEY_F13 = 302,
    KEY_F14 = 303,
    KEY_F15 = 304,
    KEY_F16 = 305,
    KEY_F17 = 306,
    KEY_F18 = 307,
    KEY_F19 = 308,
    KEY_F20 = 309,
    KEY_F21 = 310,
    KEY_F22 = 311,
    KEY_F23 = 312,
    KEY_F24 = 313,
    KEY_F25 = 314,
    KEY_KP_0 = 320,
    KEY_KP_1 = 321,
    KEY_KP_2 = 322,
    KEY_KP_3 = 323,
    KEY_KP_4 = 324,
    KEY_KP_5 = 325,
    KEY_KP_6 = 326,
    KEY_KP_7 = 327,
    KEY_KP_8 = 328,
    KEY_KP_9 = 329,
    KEY_KP_DECIMAL = 330,
    KEY_KP_DIVIDE = 331,
    KEY_KP_MULTIPLY = 332,
    KEY_KP_SUBTRACT = 333,
    KEY_KP_ADD = 334,
    KEY_KP_ENTER = 335,
    KEY_KP_EQUAL = 336,
    KEY_LEFT_SHIFT = 340,
    KEY_LEFT_CONTROL = 341,
    KEY_LEFT_ALT = 342,
    KEY_LEFT_SUPER = 343,
    KEY_RIGHT_SHIFT = 344,
    KEY_RIGHT_CONTROL = 345,
    KEY_RIGHT_ALT = 346,
    KEY_RIGHT_SUPER = 347,
    KEY_MENU = 348,
};

enum NXAPI key_action_constant {
    KEY_RELEASE = GLFW_RELEASE,
    KEY_PRESS = GLFW_PRESS,
    KEY_REPEAT = GLFW_REPEAT,
};

enum NXAPI key_mods_constant {
    KEY_MOD_SHIFT = GLFW_MOD_SHIFT,
    KEY_MOD_CTRL = GLFW_MOD_CONTROL,
    KEY_MOD_ALT = GLFW_MOD_ALT,
    KEY_MOD_SUPER = GLFW_MOD_SUPER,
};

NXAPI void test_font();

// struct mesh_t {};

} // namespace nx::gfx

#endif