#ifndef NX_LOG_H
#define NX_LOG_H

#include <boost/preprocessor/comparison/greater_equal.hpp>

#include <niu2x/api.h>

#ifndef NX_LOG_LEVEL
    #define NX_LOG_LEVEL 5
#endif

#if BOOST_PP_GREATER_EQUAL(NX_LOG_LEVEL, 5)
    #define NX_LOG_T(...) NX_LOG_WRITE(trace, ##__VA_ARGS__)
#else
    #define NX_LOG_T(...)
#endif

#if BOOST_PP_GREATER_EQUAL(NX_LOG_LEVEL, 4)
    #define NX_LOG_D(...) NX_LOG_WRITE(debug, ##__VA_ARGS__)
#else
    #define NX_LOG_D(...)
#endif

#if BOOST_PP_GREATER_EQUAL(NX_LOG_LEVEL, 3)
    #define NX_LOG_W(...) NX_LOG_WRITE(warning, ##__VA_ARGS__)
#else
    #define NX_LOG_W(...)
#endif

#if BOOST_PP_GREATER_EQUAL(NX_LOG_LEVEL, 2)
    #define NX_LOG_E(...) NX_LOG_WRITE(error, ##__VA_ARGS__)
#else
    #define NX_LOG_E(...)
#endif

#if BOOST_PP_GREATER_EQUAL(NX_LOG_LEVEL, 1)
    #define NX_LOG_F(...) NX_LOG_WRITE(fatal, ##__VA_ARGS__)
#else
    #define NX_LOG_F(...)
#endif

#define NX_LOG_WRITE(p_level, ...)                                             \
    nx::log::write(nx::log::level::p_level, __FILE__, __LINE__, ##__VA_ARGS__)

namespace nx {

struct API log {
    enum class level {
        trace,
        debug,
        warning,
        error,
        fatal,
    };

    static void write(level p_level, const char* filename, int line,
        const char* message, ...);
};

} // namespace nx

#endif