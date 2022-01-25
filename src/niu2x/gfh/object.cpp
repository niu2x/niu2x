#include "gfh.h"

#define object_create(p_type)                                                  \
    ({                                                                         \
        auto* obj = p_type##_pool.alloc();                                     \
        NX_ASSERT(obj != nullptr, "too many object");                          \
        obj->type = object_type::p_type;                                       \
        obj;                                                                   \
    })

#define object_destroy(game_object) game_object##_pool.free(game_object);

namespace nx::gfh {

static void component_setup(transform_t* com) { unused(com); }
static void component_cleanup(transform_t* com) { unused(com); }

static void transform_destroy(transform_t* transform)
{
    component_cleanup(transform);
    object_destroy(transform);
}

static void game_object_destroy(game_object_t* game_object)
{
    hashtab_cleanup(&(game_object->component_registry));
    object_destroy(game_object);
}

game_object_t* game_object_create()
{
    auto game_object = object_create(game_object);
    hashtab_setup(&(game_object->component_registry), 4);
    return game_object;
}

transform_t* transform_create()
{
    auto* transform = object_create(transform);
    component_setup(transform);
    return transform;
}

void destroy(object_t* obj)
{
    if (!obj)
        return;

#define CASE(enum_t, obj_t, destroy_method)                                    \
    case object_type::enum_t: {                                                \
        destroy_method(reinterpret_cast<obj_t*>(obj));                         \
        break;                                                                 \
    }

    switch (obj->type) {
        CASE(game_object, game_object_t, game_object_destroy)
        CASE(transform, transform_t, transform_destroy)
    }

#undef CASE
}

} // namespace nx::gfh