#include <niu2x/gfx.h>

#include "niu2x/utils.h"
#include "niu2x/list_head.h"
#include "niu2x/assert.h"
#include "niu2x/freelist.h"

namespace nx::gfx {

namespace {
struct object_type {
    enum { vertex_buffer, program };
};
} // namespace

#define create_object(list, type_t)                                            \
    {                                                                          \
        auto id = list.alloc();                                                \
        NX_ASSERT(id != list.nil, "too many object");                          \
        auto* obj = list.get(id);                                              \
        obj->type = object_type::type_t;                                       \
        obj->id = id;                                                          \
        obj;                                                                   \
    }

#define destroy_object(list, obj) list.free(obj->id);

static freelist<vertex_buffer_t, 1024> vertex_buffer_freelist;
static freelist<program_t, 1024> program_freelist;

size_t vertex_sizeof(vertex_layout_t layout)
{
    size_t size = 0;
    for (int i = 0; i < 16; i++, layout >>= 4) {
        auto attr = (vertex_attr_type)(layout & 0xF);
        switch (attr) {
            case vertex_attr_type::nil: {
                break;
            }
            case vertex_attr_type::position: {
                // vec3
                size += 3 * sizeof(GLfloat);
                break;
            }
            case vertex_attr_type::color: {
                // vec4
                size += 4 * sizeof(GLfloat);
                break;
            }
            case vertex_attr_type::normal: {
                // vec3
                size += 3 * sizeof(GLfloat);
                break;
            }
            case vertex_attr_type::uv: {
                // vec4
                size += 4 * sizeof(GLfloat);
                break;
            }
        }
    }
    return size;
}

vertex_buffer_t* create_vertex_buffer(
    vertex_layout_t layout, uint32_t vertices_count, void* data)
{
    auto* obj = (create_object(vertex_buffer_freelist, vertex_buffer));

    glGenBuffers(1, &(obj->name));
    obj->layout = layout;

    glBindBuffer(GL_ARRAY_BUFFER, obj->name);
    size_t buffer_size = vertex_sizeof(layout) * vertices_count;
    glBufferData(GL_ARRAY_BUFFER, buffer_size, data, GL_DYNAMIC_DRAW);

    return obj;
}

static void destroy_vertex_buffer(vertex_buffer_t* obj)
{
    glDeleteBuffers(1, &(obj->name));
    destroy_object(vertex_buffer_freelist, obj);
}

static GLuint create_shader(GLenum shader_type, const char* source_code)
{
    auto name = glCreateShader(shader_type);
    glShaderSource(name, 1, &source_code, nullptr);
    glCompileShader(name);
    GLint status;
    glGetShaderiv(name, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        GLint max_len;
        glGetShaderiv(name, GL_INFO_LOG_LENGTH, &max_len);
        std::vector<GLchar> log(max_len);
        glGetShaderInfoLog(name, max_len, &max_len, &log[0]);
        NX_LOG_E("shader compile failed: %s", log.data());
        glDeleteShader(name);
        name = 0;
    }
    return name;
}
static void destroy_shader(GLuint name) { glDeleteShader(name); }

static GLuint create_program(GLuint vert, GLuint frag)
{
    auto name = glCreateProgram();
    glAttachShader(name, vert);
    glAttachShader(name, frag);

    glLinkProgram(name);

    GLint status = 0;
    glGetProgramiv(name, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        GLint max_len = 0;
        glGetProgramiv(name, GL_INFO_LOG_LENGTH, &max_len);
        std::vector<GLchar> log(max_len);
        glGetProgramInfoLog(name, max_len, &max_len, &log[0]);
        NX_LOG_E("program link failed: %s", log.data());
        glDeleteProgram(name);
        name = 0;
    }
    return name;
}
static void destroy_program(GLuint name) { glDeleteProgram(name); }

program_t* create_program(const char* vert, const char* frag)
{

    auto vert_name = create_shader(GL_VERTEX_SHADER, vert);
    NX_ASSERT(vert_name != 0, "vert compile fail");

    auto frag_name = create_shader(GL_FRAGMENT_SHADER, frag);
    NX_ASSERT(frag_name != 0, "frag compile fail");

    auto progame_name = create_program(vert_name, frag_name);
    NX_ASSERT(progame_name != 0, "program link fail");

    destroy_shader(vert_name);
    destroy_shader(frag_name);

    auto* obj = (create_object(program_freelist, program));
    obj->name = progame_name;
    return obj;
}

static void destroy_program(program_t* obj)
{

    destroy_program(obj->name);
    destroy_object(program_freelist, obj);
}

void destroy(object_t* obj)
{
#define CASE(enum_t, obj_t, destroy_method)                                    \
    case object_type::enum_t: {                                                \
        destroy_method(reinterpret_cast<obj_t*>(obj));                         \
        break;                                                                 \
    }

    switch (obj->type) {
        CASE(vertex_buffer, vertex_buffer_t, destroy_vertex_buffer)
        CASE(program, program_t, destroy_program)
    }

#undef CASE
}

vertex_layout_t vertex_layout(vertex_attr_type a0, vertex_attr_type a1,
    vertex_attr_type a2, vertex_attr_type a3, vertex_attr_type a4,
    vertex_attr_type a5, vertex_attr_type a6, vertex_attr_type a7,
    vertex_attr_type a8, vertex_attr_type a9, vertex_attr_type a10,
    vertex_attr_type a11, vertex_attr_type a12, vertex_attr_type a13,
    vertex_attr_type a14, vertex_attr_type a15)
{
    return (((vertex_layout_t)(a0)&0xF) << (4 * 0))
        | (((vertex_layout_t)(a1)&0xF) << (4 * 1))
        | (((vertex_layout_t)(a2)&0xF) << (4 * 2))
        | (((vertex_layout_t)(a3)&0xF) << (4 * 3))
        | (((vertex_layout_t)(a4)&0xF) << (4 * 4))
        | (((vertex_layout_t)(a5)&0xF) << (4 * 5))
        | (((vertex_layout_t)(a6)&0xF) << (4 * 6))
        | (((vertex_layout_t)(a7)&0xF) << (4 * 7))
        | (((vertex_layout_t)(a8)&0xF) << (4 * 8))
        | (((vertex_layout_t)(a9)&0xF) << (4 * 9))
        | (((vertex_layout_t)(a10)&0xF) << (4 * 10))
        | (((vertex_layout_t)(a11)&0xF) << (4 * 11))
        | (((vertex_layout_t)(a12)&0xF) << (4 * 12))
        | (((vertex_layout_t)(a13)&0xF) << (4 * 13))
        | (((vertex_layout_t)(a14)&0xF) << (4 * 14))
        | (((vertex_layout_t)(a15)&0xF) << (4 * 15));
}

} // namespace nx::gfx
