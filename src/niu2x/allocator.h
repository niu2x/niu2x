#ifndef NIU2X_ALLOCATOR_H
#define NIU2X_ALLOCATOR_H

#include <CAllocator.h>
#include <FreeListAllocator.h>
#include <StackAllocator.h>
#include <PoolAllocator.h>
#include <LinearAllocator.h>

namespace niu2x::mm {

using pool_memory_t   = PoolAllocator;
using linear_memory_t = LinearAllocator;
using stack_memory_t  = StackAllocator;
using memory_t        = CAllocator;

namespace details {

template <class T>
class allocator_adapter {
public:
    allocator_adapter(T &delegate)
        : delegate_(delegate) {}
    ~allocator_adapter() {}

    void *allocate(const std::size_t size, const std::size_t alignment = 0) {
        return delegate_.Allocate(size, alignment);
    }
    void free(void *ptr) {
        delegate_.Free(ptr);
    }

private:
    T &delegate_;
};

} // namespace details

using allocator = details::allocator_adapter<Allocator>;

extern memory_t memory;

} // namespace niu2x::mm

#endif