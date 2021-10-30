#include <niu2x/io.h>

#include "io.h"

namespace nx::io {

source::source(std::istream& stream)
: delegate_(&stream)
{
}

source::~source() { }

int source::read(void* data, size_t bytes)
{
    return std::visit(
        [data, bytes](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            return io::read(std::forward<T>(arg), data, bytes);
        },
        delegate_);
}

} // namespace nx::io