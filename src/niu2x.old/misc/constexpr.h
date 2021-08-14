#ifndef NX_MISC_CONSTEXPR_H
#define NX_MISC_CONSTEXPR_H

namespace nx::misc {

namespace details {
constexpr bool is_digit(char c) { return c <= '9' && c >= '0'; }

constexpr int stoi_impl(const char* str, int value = 0)
{
    return *str ? is_digit(*str) ? stoi_impl(str + 1, (*str - '0') + value * 10)
                                 : throw "compile-time-error: not a digit"
                : value;
}
} // namespace details

constexpr int stoi(const char* str) { return details::stoi_impl(str); }

} // namespace nx::misc

#endif
