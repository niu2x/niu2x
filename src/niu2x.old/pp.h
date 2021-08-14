#ifndef NX_PP_H
#define NX_PP_H

#define NX_PP_BEGIN

#define NX_PP_ADD(x, y) NX_PP_ADD_##x##_##y
#include <niu2x/pp/add.h>

#define NX_PP_GREATER_EQUAL(x, y) NX_PP_GREATER_EQUAL_##x##_##y
#include <niu2x/pp/greater_equal.h>

#undef NX_PP_BEGIN

#endif