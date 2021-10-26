#ifndef NX_UTILS_LIMIT_H
#define NX_UTILS_LIMIT_H

#include <cstddef>

#include <niu2x/api.h>

namespace nx {

// limits
struct API limits {
#define LIMIT(type, name, value) static constexpr type name = value;

    LIMIT(size_t, max_path, 1024)
    LIMIT(size_t, max_varname, 256)
    LIMIT(size_t, max_error_message, 512)

#undef LIMIT
};

} // namespace nx

#endif