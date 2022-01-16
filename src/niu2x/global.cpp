#include "niu2x/global.h"
#include "niu2x/utils.h"

namespace nx {

#if defined(NX_MAX_MEM)
static freelist_memory_t memory(NX_MAX_MEM);
#else
static mallocfree_memory_t memory;
#endif

memory_proxy_t global::mem(&memory);

} // namespace nx

#include "hashtab.h"