#ifndef NX_UTILS_H
#define NX_UTILS_H

#include <string.h>

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <functional>

#include <boost/noncopyable.hpp>

#include <niu2x/api.h>
#include <niu2x/log.h>
#include <niu2x/misc/constexpr.h>

namespace nx {

// limits
struct limits {
#define LIMIT(type, name, value) static constexpr type name = value;

    LIMIT(size_t, max_path, 1024)

#undef LIMIT
};

// unused
inline void unused() { }

template <class FirstParam, class... Params>
inline void unused(FirstParam&& first_param, Params&&... params)
{
    (void)first_param;
    unused(std::forward<Params>(params)...);
}

// noncopyable
using noncopyable = boost::noncopyable;

// literals
inline constexpr size_t operator"" _k(const char* n)
{
    return misc::stoi(n) << 10;
}
inline constexpr size_t operator"" _m(const char* n)
{
    return misc::stoi(n) << 20;
}
inline constexpr size_t operator"" _g(const char* n)
{
    return misc::stoi(n) << 30;
}

// math utils
template <class T> inline T min(const T& a, const T& b)
{
    return a < b ? a : b;
}

template <class T> inline T max(const T& a, const T& b)
{
    return a > b ? a : b;
}

#define NX_ARRAY_SIZE(array) sizeof(array)/sizeof(array[0])

// exception
class exception : public std::exception {
public:
    exception(const char* msg, const char* file, uint32_t line)
    {
        const char* fmt = "%s\nat: %s line: %d";
        snprintf(msg_, buffer_size, fmt, msg, file, line);
    }
    virtual ~exception() { }
    virtual const char* what() const noexcept override { return msg_; }

private:
    enum { buffer_size = 512 };
    char msg_[buffer_size];
};

} // namespace nx

#define NX_THROW(message) throw nx::exception((message), __FILE__, __LINE__)

#define NX_ASSERT(condition, message) \
if(!(condition)){   \
    NX_THROW((message)); \
}

// memref

// struct memref {
//     void *ptr
// };

template <class T>
struct arrayref {
    T* base;
    size_t size;
};

#endif