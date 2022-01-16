#include <niu2x/memory.h>

#include <cstdlib>
#include <algorithm>

#include "niu2x/memory-allocators/FreeListAllocator.h"

namespace nx {

freelist_memory_t::freelist_memory_t(size_t mem_size)
{
    auto* allocator
        = new FreeListAllocator(mem_size, FreeListAllocator::FIND_FIRST);
    allocator->Init();
    delegate_ = allocator;
}

freelist_memory_t::~freelist_memory_t()
{
    auto* allocator = reinterpret_cast<FreeListAllocator*>(delegate_);
    delete allocator;
}

void* freelist_memory_t::allocate(size_t size) noexcept
{
    auto* allocator = reinterpret_cast<FreeListAllocator*>(delegate_);
    // FreeListAllocator-Node's size is 16.
    return allocator->Allocate(std::max(size, 16ul), alignof(std::max_align_t));
}

void freelist_memory_t::free(void* ptr) noexcept
{
    auto* allocator = reinterpret_cast<FreeListAllocator*>(delegate_);
    allocator->Free(ptr);
}

uint64_t freelist_memory_t::used() noexcept
{

    auto* allocator = reinterpret_cast<FreeListAllocator*>(delegate_);

    return allocator->used();
}

} // namespace nx