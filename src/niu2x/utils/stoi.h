#ifndef NX_UTILS_STOI_H
#define NX_UTILS_STOI_H

namespace nx {

namespace stoi_details {
constexpr inline bool is_digit(char c) { return c <= '9' && c >= '0'; }
} // namespace stoi_details

constexpr inline int stoi(const char* str, int value = 0)
{
    return *str ? stoi_details::is_digit(*str)
            ? stoi(str + 1, (*str - '0') + value * 10)
            : throw "compile-time-error: not a digit"
                : value;
}

} // namespace nx

#endif
