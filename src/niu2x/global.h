#ifndef NX_GLOBAL_H
#define NX_GLOBAL_H

#include <niu2x/memory.h>

namespace nx {

struct global {
    static memory_proxy mem;
};

#define NX_ALLOC(type, n) ((type*)(nx::global::mem.allocate(sizeof(type) * n)))
#define NX_FREE(ptr)      nx::global::mem.free(ptr)

} // namespace nx

#endif