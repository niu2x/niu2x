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

vertex_buffer_t* create_vertex_buffer()
{
    auto* obj = (create_object(vertex_buffer_freelist, vertex_buffer));

    glGenBuffers(1, &(obj->name));

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

} // namespace nx::gfx
