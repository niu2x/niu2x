#include <niu2x/gfx.h>

namespace nx::gfx {

static mallocfree_memory memory;

memory_proxy mem(&memory);

} // namespace nx::gfx