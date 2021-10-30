#include <niu2x/io.h>

namespace nx::io {

namespace {
mallocfree_memory memory;
}

memory_proxy mem(&memory);

} // namespace nx::io