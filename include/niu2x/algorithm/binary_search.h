#ifndef NX_ALGORITHM_BINARY_SEARCH_H
#define NX_ALGORITHM_BINARY_SEARCH_H

#include <type_traits>
#include <cstdio>

#include <niu2x/api.h>

namespace nx::algo {

using compare_t = int (*)(const void*, const void*);

NXAPI int binary_search(const void* base, const int nr, const int stride,
    const void* target, const compare_t compare);

template <class T>
inline typename std::enable_if<not std::is_arithmetic<T>::value, int>::type
compare(const void* a, const void* b)
{
    if (*((const T*)a) == *((const T*)b))
        return 0;
    if (*((const T*)a) < *((const T*)b))
        return -1;
    return 1;
}

template <class T>
inline typename std::enable_if<std::is_arithmetic<T>::value, int>::type compare(
    const void* a, const void* b)
{
    return (*((const T*)a) - *((const T*)b));
}

template <class T>
inline int binary_search(const T* base, const int nr, const T* target)
{
    return binary_search(base, nr, sizeof(T), target, compare<T>);
}

} // namespace nx::algo

#endif
