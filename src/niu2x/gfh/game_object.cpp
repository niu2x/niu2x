#include "gfh.h"

namespace nx::gfh {

static void game_object_setup(game_object_t* self)
{
    hashtab_setup(&(self->components));
    self->parent = nullptr;
    self->ref = 1;
    list_init(&(self->children));
}
static void game_object_cleanup(game_object_t* self)
{
    game_object_remove_children(self);
    hashtab_cleanup(&(self->components));
}

void game_object_remove_children(game_object_t* self)
{
    NX_LIST_FOR_EACH(ptr, &(self->children))
    {
        auto child = NX_LIST_ENTRY(ptr, game_object_t, self_in_children);
        child->parent = nullptr;
        game_object_release(child);
    }
    list_init(&(self->children));
}

game_object_t* game_object_create()
{
    auto game_object = game_object_pool.alloc();
    game_object_setup(game_object);
    return game_object;
}

void destroy(game_object_t* self)
{
    game_object_cleanup(self);
    game_object_pool.free(self);
}

void game_object_add_child(game_object_t* self, game_object_t* child)
{
    child->parent = self;
    game_object_retain(child);
    list_add_tail(&(child->self_in_children), &(self->children));
}

} // namespace nx::gfh