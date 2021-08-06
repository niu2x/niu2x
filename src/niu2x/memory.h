#ifndef NX_MEMORY_H
#define NX_MEMORY_H

#if defined(_WIN32) || defined(_WIN64)
    #pragma warning(disable : 4275)
#endif

#include <niu2x/memory-allocators/CAllocator.h>
#include <niu2x/memory-allocators/FreeListAllocator.h>
#include <niu2x/memory-allocators/LinearAllocator.h>
#include <niu2x/memory-allocators/PoolAllocator.h>
#include <niu2x/memory-allocators/StackAllocator.h>
#include <niu2x/api.h>
#include <niu2x/misc/noncopyable.h>

namespace nx::mm {

class API memory_t : private misc::noncopyable, public CAllocator {
};

class API freelist_memory_t : private misc::noncopyable,
                              public FreeListAllocator {
public:
    freelist_memory_t(size_t total_size);
    ~freelist_memory_t();
};

namespace details {

    template <class T> class API allocator_adapter {
    public:
        allocator_adapter(T& delegate)
        : delegate_(delegate)
        {
        }
        ~allocator_adapter() { }

        void* allocate(const std::size_t size, const std::size_t alignment = 0)
        {
            return delegate_.Allocate(size, alignment);
        }
        void free(void* ptr) { delegate_.Free(ptr); }

    private:
        T& delegate_;
    };

} // namespace details

using allocator = details::allocator_adapter<Allocator>;

extern API memory_t memory;

} // namespace nx::mm

#endif