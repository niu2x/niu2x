#ifndef NX_LOG_H
#define NX_LOG_H

#include <niu2x/api.h>
#include <niu2x/pp.h>

#ifndef NX_LOG_LEVEL
    #define NX_LOG_LEVEL 4
#endif

#if NX_PP_GREATER_EQUAL(NX_LOG_LEVEL, 5)
    #define NX_LOG_T(...) NX_LOG_WRITE(trace, ##__VA_ARGS__)
#else
    #define NX_LOG_T(...)
#endif

#if NX_PP_GREATER_EQUAL(NX_LOG_LEVEL, 4)
    #define NX_LOG_D(...) NX_LOG_WRITE(debug, ##__VA_ARGS__)
#else
    #define NX_LOG_D(...)
#endif

#if NX_PP_GREATER_EQUAL(NX_LOG_LEVEL, 3)
    #define NX_LOG_W(...) NX_LOG_WRITE(warning, ##__VA_ARGS__)
#else
    #define NX_LOG_W(...)
#endif

#if NX_PP_GREATER_EQUAL(NX_LOG_LEVEL, 2)
    #define NX_LOG_E(...) NX_LOG_WRITE(error, ##__VA_ARGS__)
#else
    #define NX_LOG_E(...)
#endif

#if NX_PP_GREATER_EQUAL(NX_LOG_LEVEL, 1)
    #define NX_LOG_F(...) NX_LOG_WRITE(fatal, ##__VA_ARGS__)
#else
    #define NX_LOG_F(...)
#endif

#define NX_LOG_WRITE(p_level, ...)                                             \
    nx::log::write(nx::log::level::p_level, __FILE__, __LINE__, ##__VA_ARGS__)

namespace nx {

struct API log {
    enum level {
        none,
        fatal,
        error,
        warning,
        debug,
        trace,
    };

    static void write(
        int p_level, const char* filename, int line, const char* message, ...);

private:
    static int current_level_;
};

} // namespace nx

#endif