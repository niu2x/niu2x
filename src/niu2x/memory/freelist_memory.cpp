#include <niu2x/memory.h>

#include <cstdlib>

#include <niu2x/memory-allocators/FreeListAllocator.h>
#include <niu2x/utils.h>

namespace nx {

freelist_memory::freelist_memory(size_t capacity)
{
    delegate_ = std::make_unique<FreeListAllocator>(
        capacity, FreeListAllocator::FIND_FIRST);
    delegate_->Init();
}
freelist_memory::~freelist_memory() { }

void* freelist_memory::allocate(size_t size)
{
    return delegate_->Allocate(size, alignof(std::max_align_t));
}

void freelist_memory::free(void* ptr) { delegate_->Free(ptr); }

} // namespace nx