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
#include <memory>
#include <limits>

#include <niu2x/api.h>
#include <niu2x/log.h>
#include <niu2x/memory.h>
#include <niu2x/misc/constexpr.h>
#include <niu2x/misc/string_utils.h>
#include <niu2x/misc/noncopyable.h>

namespace nx {

// limits
struct API limits {
#define LIMIT(type, name, value) static constexpr type name = value;

    LIMIT(size_t, max_path, 1024)
    LIMIT(size_t, max_varname, 256)

#undef LIMIT
};

// unused
inline API void unused() { }

template <class FirstParam, class... Params>
inline void unused(FirstParam&& first_param, Params&&... params)
{
    (void)first_param;
    unused(std::forward<Params>(params)...);
}

// noncopyable
using noncopyable = misc::noncopyable;

// literals
inline API constexpr size_t operator"" _k(const char* n)
{
    return misc::stoi(n) << 10;
}
inline API constexpr size_t operator"" _m(const char* n)
{
    return misc::stoi(n) << 20;
}
inline API constexpr size_t operator"" _g(const char* n)
{
    return misc::stoi(n) << 30;
}

// math utils
template <class T>
inline API T min(const T& a, const T& b)
{
    return a < b ? a : b;
}

template <class T>
inline API T max(const T& a, const T& b)
{
    return a > b ? a : b;
}

// exception
class API exception : public std::exception {
public:
    exception(const char* msg, const char* file, uint32_t line)
    {
        const char* fmt = "%s\nat: %s line: %d";
        snprintf(msg_, buffer_size, fmt, msg, file, line);
    }

    exception(const std::string& sz, const char* file, uint32_t line)
    : exception(sz.c_str(), file, line)
    {
    }

    virtual ~exception() { }
    virtual const char* what() const noexcept override { return msg_; }

private:
    enum { buffer_size = 512 };
    char msg_[buffer_size];
};

using string_utils = misc::string_utils;

template <class T>
struct API arrayref {
    T* base;
    size_t size;
};

template <class T>
void API swap(T& a, T& b)
{
    T tmp = a;
    a = b;
    b = tmp;
}

// memref
struct API memref {
    void* base;
    size_t size;
};

// template<class T>
// struct nil {
//     static constexpr T v;
// };
//
template <class T>
struct API destructor {
    static void destroy(T* obj) { obj->~T(); }
};

enum API status {
    ok,
    again,
    eof,
    fail,
};

using rid = uint64_t;
constexpr rid nil = 0;

extern API mm::allocator default_allocator;

} // namespace nx

#define NX_MIN(a, b) (a) < (b) ? (a) : (b)
#define NX_MAX(a, b) (a) > (b) ? (a) : (b)

#define NX_ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

#define NX_THROW(message) throw nx::exception((message), __FILE__, __LINE__)

#define NX_ASSERT(condition, message)                                          \
    if (!(condition)) {                                                        \
        NX_THROW((message));                                                   \
    }

#define NX_INIT_FUNC(f)                                                        \
    static void nx_init_func();                                                \
    namespace {                                                                \
    struct nx_init_wrapper {                                                   \
        nx_init_wrapper() { nx_init_func(); }                                  \
        static nx_init_wrapper instance_;                                      \
    };                                                                         \
    nx_init_wrapper nx_init_wrapper::instance_;                                \
    }                                                                          \
    static void nx_init_func()

#endif