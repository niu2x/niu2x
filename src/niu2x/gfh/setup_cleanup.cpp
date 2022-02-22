#include <niu2x/gfh.h>

namespace nx::gfh {

static int64_t counter = 0;

static void __setup() { }
static void __cleanup() { }

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