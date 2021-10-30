#include <niu2x/io.h>

#include "io.h"

namespace nx::io {

sink::sink(std::ostream& stream)
: delegate_(&stream)
{
}

sink::~sink() { }

int sink::write(const void* data, size_t bytes)
{
    return std::visit(
        [data, bytes](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            return io::write(std::forward<T>(arg), data, bytes);
        },
        delegate_);
}

} // namespace nx::io