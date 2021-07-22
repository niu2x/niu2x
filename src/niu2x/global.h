#ifndef NX_GLOBAL_H
#define NX_GLOBAL_H

#include <niu2x/api.h>
#include <niu2x/memory.h>

namespace nx {

struct API global {
    static mm::allocator allocator;
};

} // namespace nx

#endif