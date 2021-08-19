#ifndef NX_PP_H
#define NX_PP_H

#if defined(_WIN32) || defined(_WIN64)
    #pragma warning(disable : 4067)
#endif

#define NX_PP_BEGIN

#define NX_PP_VA_ARGS_COUNT(...)                                               \
    NX_PP_VA_ARGS_COUNT_HELPER(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1)
#define NX_PP_VA_ARGS_COUNT_HELPER(_8, _7, _6, _5, _4, _3, _2, _1, x, ...) x

#define NX_PP_CONCAT(...)                                                      \
    NX_PP_CONCAT_X0(NX_PP_VA_ARGS_COUNT(__VA_ARGS__), __VA_ARGS__)
#define NX_PP_CONCAT_X0(n, ...) NX_PP_CONCAT_X1(n, __VA_ARGS__)
#define NX_PP_CONCAT_X1(n, ...) NX_PP_CONCAT_##n(__VA_ARGS__)
#define NX_PP_CONCAT_X2(a, b) a##b
#define NX_PP_CONCAT_X3(a, b, ...) NX_PP_CONCAT_X2(a##b, __VA_ARGS__)
#define NX_PP_CONCAT_X4(a, b, ...) NX_PP_CONCAT_X3(a##b, __VA_ARGS__)

#define NX_PP_ADD(x, y) NX_PP_ADD_##x##_##y
#include <niu2x/pp/add.h>

#define NX_PP_GREATER_EQUAL(x, y) NX_PP_CONCAT(NX_PP_GREATER_EQUAL_, x, _, y)
#include <niu2x/pp/greater_equal.h>

#undef NX_PP_BEGIN

#endif