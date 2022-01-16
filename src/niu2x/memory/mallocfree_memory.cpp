#include <niu2x/memory.h>

#include <cstdlib>

namespace nx {

mallocfree_memory_t::mallocfree_memory_t() { }
mallocfree_memory_t::~mallocfree_memory_t() { }

void* mallocfree_memory_t::allocate(size_t size) noexcept
{
    return ::malloc(size);
}

void mallocfree_memory_t::free(void* ptr) noexcept { ::free(ptr); }

} // namespace nx