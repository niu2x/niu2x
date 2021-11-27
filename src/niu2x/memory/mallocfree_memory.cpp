#include <niu2x/memory.h>

#include <cstdlib>

namespace nx {

mallocfree_memory::mallocfree_memory() { }
mallocfree_memory::~mallocfree_memory() { }

void* mallocfree_memory::allocate(size_t size) noexcept
{
    return ::malloc(size);
}

void mallocfree_memory::free(void* ptr) noexcept { ::free(ptr); }

} // namespace nx