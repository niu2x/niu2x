#ifndef NX_GLOBAL_H
#define NX_GLOBAL_H

#include <niu2x/memory.h>

namespace nx {

struct global {
    static memory_proxy_t mem;
};

#define NX_ALLOC(type, n)                                                      \
    ((type*)(nx::global::mem.allocate(sizeof(type) * (n))))
#define NX_FREE(ptr)      nx::global::mem.free(ptr)

#define NX_DELETE(obj_ptr) details::nx_delete(obj_ptr);
#define NX_NEW(T, ...)     details::nx_new<T>(##__VA_ARGS__)

namespace details {

template <class T, class... Params>
T* nx_new(Params&&... params)
{
    T* obj = NX_ALLOC(T, 1);
    return new (obj) T(std::forward<Params>(params)...);
}

template <class T>
void nx_delete(T* ptr)
{
    ptr->~T();
    NX_FREE(ptr);
}

} // namespace details

} // namespace nx

#endif