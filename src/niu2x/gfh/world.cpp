#include "gfh.h"

namespace nx::gfh {

void world_setup(world_t* self) { self->root = nullptr; }

void world_cleanup(world_t* self)
{
    if (self->root) {
        game_object_release(self->root);
        self->root = nullptr;
    }
}

} // namespace nx::gfh