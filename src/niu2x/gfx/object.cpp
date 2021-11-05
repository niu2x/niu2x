#include <niu2x/gfx.h>

#include "niu2x/utils.h"
#include "niu2x/list_head.h"
#include "niu2x/assert.h"
#include "niu2x/freelist.h"

namespace nx::gfx {

namespace {
struct object_type {
    enum { vertex_buffer };
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

static size_t vertex_sizeof(vertex_layout_t layout)
{
    size_t size = 0;
    for (int i = 0; i < 8; i++, layout >>= 4) {
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
    vertex_layout_t layout, uint32_t vertices_count)
{
    auto* obj = (create_object(vertex_buffer_freelist, vertex_buffer));

    glGenBuffers(1, &(obj->name));

    glBindBuffer(GL_ARRAY_BUFFER, obj->name);
    size_t buffer_size = vertex_sizeof(layout) * vertices_count;
    glBufferData(GL_ARRAY_BUFFER, buffer_size, 0, GL_DYNAMIC_DRAW);

    return obj;
}

static void destroy_vertex_buffer(vertex_buffer_t* obj)
{
    glDeleteBuffers(1, &(obj->name));
    destroy_object(vertex_buffer_freelist, obj);
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
