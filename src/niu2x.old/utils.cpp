#include "utils.h"

namespace nx {

static mm::memory_t backend;
mm::allocator default_allocator(backend);

} // namespace nx