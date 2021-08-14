#include <niu2x/memory.h>

namespace nx::mm {

memory_t memory;

freelist_memory_t::freelist_memory_t(size_t total_size)
: FreeListAllocator(total_size, FreeListAllocator::FIND_FIRST)
{
    Init();
}
freelist_memory_t::~freelist_memory_t() { }

} // namespace nx::mm