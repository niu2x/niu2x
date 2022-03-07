#include "gfh.h"

namespace nx::gfh {

void world_setup(world_t* self)
{
    self->root = game_object_create();
}

void world_cleanup(world_t* self)
{
    game_object_release(self->root);
    self->root = nullptr;
}

void world_update(world_t* self, double dt) { unused(dt, self); }

} // namespace nx::gfh