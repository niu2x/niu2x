#include "gfh.h"

namespace nx::gfh {

NX_OBJ_POOL_DEFINE2(game_object, 0);

static int64_t counter = 0;

static void __setup() { }
static void __cleanup() { game_object_pool.clear(); }

void setup()
{
    if (counter++ == 0) {
        __setup();
    }
}

void cleanup()
{
    if (--counter == 0) {
        __cleanup();
    }
}

} // namespace nx::gfh