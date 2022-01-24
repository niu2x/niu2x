#ifndef NX_OBJECT_H
#define NX_OBJECT_H

#include <niu2x/freelist2.h>

namespace nx {

struct NXAPI object_t {
    void* freelist;
    freelist_rid_t rid;
};

} // namespace nx

#endif