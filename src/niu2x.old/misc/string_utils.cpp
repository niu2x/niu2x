#include <niu2x/misc/string_utils.h>

// std::string s = "scott>=tiger>=mushroom";
// std::string delimiter = ">=";

namespace nx::misc {

std::vector<std::string> string_utils::split(
    const std::string_view& sz, uint8_t delimiter)
{

    std::vector<std::string> results;
    size_t pos = 0, prev_pos = 0;
    std::string token;
    while ((pos = sz.find(delimiter, prev_pos)) != std::string_view::npos) {
        results.emplace_back(sz.substr(prev_pos, pos - prev_pos));
        prev_pos = pos + 1;
    }

    if (prev_pos < sz.length()) {
        results.emplace_back(sz.substr(prev_pos, sz.length() - prev_pos));
    } else {
        results.emplace_back("");
    }

    return results;
}

} // namespace nx::misc
