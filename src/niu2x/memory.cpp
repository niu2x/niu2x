#include <niu2x/memory.h>

namespace nx::mm {

memory_t memory;

// pool_memory_t::pool_memory_t(size_t total_size, size_t chunk_size)
// : PoolAllocator(total_size, chunk_size)
// {
//     Init();
// }

// pool_memory_t::~pool_memory_t() { }

// linear_memory_t::linear_memory_t(size_t total_size)
// : LinearAllocator(total_size)
// {
//     Init();
// }
// linear_memory_t::~linear_memory_t() { }

// stack_memory_t::stack_memory_t(size_t total_size)
// : StackAllocator(total_size)
// {
//     Init();
// }
// stack_memory_t::~stack_memory_t() { }

freelist_memory_t::freelist_memory_t(size_t total_size)
: FreeListAllocator(total_size, FreeListAllocator::FIND_FIRST)
{
    Init();
}
freelist_memory_t::~freelist_memory_t() { }

} // namespace nx::mm