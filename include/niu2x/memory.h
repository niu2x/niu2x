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

class NXAPI memory_t : private boost::noncopyable {
public:
    memory_t();
    virtual ~memory_t() = 0;

    virtual void* allocate(size_t size) noexcept = 0;
    virtual void free(void* ptr) noexcept = 0;
    virtual uint64_t used() noexcept = 0;
};

class NXAPI freelist_memory_t : public memory_t {
public:
    freelist_memory_t(size_t capacity);
    virtual ~freelist_memory_t();

    virtual void* allocate(size_t size) noexcept;
    virtual void free(void* ptr) noexcept;
    virtual uint64_t used() noexcept;

private:
    void* delegate_;
};

class NXAPI mallocfree_memory_t : public memory_t {
public:
    mallocfree_memory_t();
    virtual ~mallocfree_memory_t();

    virtual void* allocate(size_t size) noexcept;
    virtual void free(void* ptr) noexcept;
    virtual uint64_t used() noexcept { return 0; }
};

class NXAPI memory_proxy_t {
public:
    memory_proxy_t(memory_t* delegate)
    : delegate_(delegate)
    {
    }
    ~memory_proxy_t() { }

    memory_proxy_t(const memory_proxy_t&) = default;
    memory_proxy_t& operator=(const memory_proxy_t&) = default;

    void* allocate(size_t size) noexcept { return delegate_->allocate(size); }
    void free(void* ptr) noexcept { delegate_->free(ptr); }

    uint64_t used() const noexcept { return delegate_->used(); }

private:
    memory_t* delegate_;
};

} // namespace nx
#endif