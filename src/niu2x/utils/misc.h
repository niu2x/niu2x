#ifndef NX_UTILS_MISC_H
#define NX_UTILS_MISC_H

#include <niu2x/api.h>

namespace nx {

template <class T>
struct arrayref {
    T* base;
    size_t size;
};

// memref
struct API memref {
    void* base;
    size_t size;
};

template <class T>
struct destructor {
    static void destroy(T* obj) { obj->~T(); }
};

} // namespace nx

#define NX_MIN(a, b) ((a) < (b) ? (a) : (b))
#define NX_MAX(a, b) ((a) > (b) ? (a) : (b))

#define NX_ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

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