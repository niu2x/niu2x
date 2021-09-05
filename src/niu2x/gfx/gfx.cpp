#include <niu2x/gfx.h>
#include <niu2x/gfx/internal.h>

namespace nx::gfx {

namespace {

mallocfree_memory default_memory;

} // namespace

memory_proxy default_allocator { &default_memory };

void set_allocator(memory_proxy allocator) { default_allocator = allocator; }

} // namespace nx::gfx