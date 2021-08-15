#ifndef NX_STRING_UTILS_H
#define NX_STRING_UTILS_H

#include <string>
#include <vector>
#include <string_view>

#include <niu2x/api.h>

namespace nx {

struct API string_utils {
    static std::vector<std::string> split(
        const std::string_view& sz, uint8_t delimiter);
};

} // namespace nx

#endif