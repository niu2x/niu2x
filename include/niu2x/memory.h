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

class API memory : private boost::noncopyable {
public:
    memory();
    virtual ~memory() = 0;

    virtual void* allocate(size_t size) = 0;
    virtual void free(void* ptr) = 0;
};

class API freelist_memory : public memory {
public:
    freelist_memory(size_t capacity);
    virtual ~freelist_memory();

    virtual void* allocate(size_t size);
    virtual void free(void* ptr);

private:
    void* delegate_;
};

class API mallocfree_memory : public memory {
public:
    mallocfree_memory();
    virtual ~mallocfree_memory();

    virtual void* allocate(size_t size);
    virtual void free(void* ptr);
};

class API memory_proxy {
public:
    memory_proxy(memory* delegate)
    : delegate_(delegate)
    {
    }
    ~memory_proxy() { }

    memory_proxy(const memory_proxy&) = default;
    memory_proxy& operator=(const memory_proxy&) = default;

    void* allocate(size_t size) { return delegate_->allocate(size); }
    void free(void* ptr) { delegate_->free(ptr); }

private:
    memory* delegate_;
};

} // namespace nx
#endif