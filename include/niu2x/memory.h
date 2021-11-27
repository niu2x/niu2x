#ifndef NX_MEMORY_H
#define NX_MEMORY_H

#if defined(_WIN32) || defined(_WIN64)
    #pragma warning(disable : 4251)
#endif

#include <memory>
#include <cstddef>

#include <boost/noncopyable.hpp>

#include <niu2x/api.h>

namespace nx {

class NXAPI memory : private boost::noncopyable {
public:
    memory();
    virtual ~memory() = 0;

    virtual void* allocate(size_t size) noexcept = 0;
    virtual void free(void* ptr) noexcept = 0;
    virtual uint64_t used() noexcept = 0;
};

class NXAPI freelist_memory : public memory {
public:
    freelist_memory(size_t capacity);
    virtual ~freelist_memory();

    virtual void* allocate(size_t size) noexcept;
    virtual void free(void* ptr) noexcept;
    virtual uint64_t used() noexcept;

private:
    void* delegate_;
};

class NXAPI mallocfree_memory : public memory {
public:
    mallocfree_memory();
    virtual ~mallocfree_memory();

    virtual void* allocate(size_t size) noexcept;
    virtual void free(void* ptr) noexcept;
    virtual uint64_t used() noexcept { return 0xFFFFFFFFFFFFFFFF; }
};

class NXAPI memory_proxy {
public:
    memory_proxy(memory* delegate)
    : delegate_(delegate)
    {
    }
    ~memory_proxy() { }

    memory_proxy(const memory_proxy&) = default;
    memory_proxy& operator=(const memory_proxy&) = default;

    void* allocate(size_t size) noexcept { return delegate_->allocate(size); }
    void free(void* ptr) noexcept { delegate_->free(ptr); }

    uint64_t used() const noexcept { return delegate_->used(); }

private:
    memory* delegate_;
};

} // namespace nx
#endif