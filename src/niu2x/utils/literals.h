#ifndef NX_UTILS_LITERALS_H
#define NX_UTILS_LITERALS_H

#include <niu2x/utils/stoi.h>

namespace nx {

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