#include "gfh.h"

namespace nx::gfh {

nx::obj_pool_t<game_object_t> game_object_pool(0);
nx::obj_pool_t<transform_t> transform_pool(0);

void setup() { }

void cleanup()
{
    game_object_pool.clear();
    transform_pool.clear();
}

} // namespace nx::gfh
