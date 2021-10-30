#ifndef NX_LIMIT_H
#define NX_LIMIT_H

#include <cstddef>

namespace nx {

// limits
struct limits {
#define LIMIT(type, name, value) static constexpr type name = value;

    LIMIT(size_t, max_path, 1024)
    LIMIT(size_t, max_varname, 256)
    LIMIT(size_t, max_error_message, 512)

#undef LIMIT
};

} // namespace nx

#endif