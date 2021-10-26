#ifndef NX_PP_H
#define NX_PP_H

#include <boost/preprocessor.hpp>

#if defined(_WIN32) || defined(_WIN64)
    #pragma warning(disable : 4067)
#endif

#define NX_PP_BEGIN

#define NX_PP_GREATER_EQUAL(x, y) BOOST_PP_GREATER_EQUAL(x, y)

#undef NX_PP_BEGIN

#endif