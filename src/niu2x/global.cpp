#include <niu2x/global.h>

namespace nx {

static mm::memory_t memory;
mm::allocator global::allocator { memory };

} // namespace nx