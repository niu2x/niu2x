#include <niu2x/gfh.h>

#include "niu2x/obj_pool.h"

namespace nx::gfh {

extern nx::obj_pool_t<game_object_t> game_object_pool;
extern nx::obj_pool_t<transform_t> transform_pool;

} // namespace nx::gfh