#ifndef NIU2X_MEMORY_H
#define NIU2X_MEMORY_H

#include <niu2x/utils.h>
#include <niu2x/memory-allocators/CAllocator.h>
#include <niu2x/memory-allocators/FreeListAllocator.h>
#include <niu2x/memory-allocators/StackAllocator.h>
#include <niu2x/memory-allocators/PoolAllocator.h>
#include <niu2x/memory-allocators/LinearAllocator.h>

namespace niu2x::mm {

class API pool_memory_t : private noncopyable, public PoolAllocator {
public:
    pool_memory_t(size_t total_size, size_t chunk_size);
    ~pool_memory_t();
};

class API linear_memory_t : private noncopyable, public LinearAllocator {
public:
    linear_memory_t(size_t total_size);
    ~linear_memory_t();
};

class API stack_memory_t : private noncopyable, public StackAllocator {
public:
    stack_memory_t(size_t total_size);
    ~stack_memory_t();
};

class API memory_t : private noncopyable, public CAllocator {};

class API freelist_memory_t : private noncopyable, public FreeListAllocator {
public:
    freelist_memory_t(size_t total_size);
    ~freelist_memory_t();
};

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

extern API memory_t memory;

} // namespace niu2x::mm

#endif