#include <niu2x/pipe.h>

namespace nx::pipe {

namespace {
mallocfree_memory memory;
}

memory_proxy mem(&memory);

} // namespace nx::pipe