#ifndef NX_SRC_UTILS_H
#define NX_SRC_UTILS_H

namespace nx {

namespace stoi_details {
constexpr inline bool is_digit(char c) { return c <= '9' && c >= '0'; }
} // namespace stoi_details

/**
 * @brief      string to int on compile time
 *
 * @param[in]  str    The string
 * @param[in]  value  default value
 *
 * @return     if str is nullptr, return default value.
 *             otherwise convert str to int.
 */
constexpr inline int stoi(const char* str, int value = 0)
{
    return *str ? stoi_details::is_digit(*str)
            ? stoi(str + 1, (*str - '0') + value * 10)
            : throw "compile-time-error: not a digit"
                : value;
}

inline constexpr size_t operator"" _k(const char* n)
{
    return nx::stoi(n) << 10;
}
inline constexpr size_t operator"" _m(const char* n)
{
    return nx::stoi(n) << 20;
}
inline constexpr size_t operator"" _g(const char* n)
{
    return nx::stoi(n) << 30;
}

} // namespace nx

#endif