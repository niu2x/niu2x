#include "niu2x/global.h"
#include "niu2x/utils.h"

namespace nx {

#if defined(NX_MAX_MEM)
static freelist_memory memory(NX_MAX_MEM);
#else
static mallocfree_memory memory;
#endif

memory_proxy global::mem(&memory);

} // namespace nx

#include "hashtab.h"