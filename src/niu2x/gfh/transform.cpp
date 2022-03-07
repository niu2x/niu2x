#include "gfh.h"

namespace nx::gfh {

static void transform_setup(transform_t* self)
{
    // self->ref = 1;
    component_setup(self);
    self->type = component_t::transform;
}

static void transform_cleanup(transform_t* self) { component_cleanup(self); }

static void destroy(transform_t* self)
{
    transform_cleanup(self);
    transform_pool.free(self);
}

transform_t* transform_create()
{
    auto transform = transform_pool.alloc();
    transform_setup(transform);
    return transform;
}

void transform_retain(transform_t* self) { component_retain(self); }

void transform_release(transform_t* self) { component_release(self); }

} // namespace nx::gfh
