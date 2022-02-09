#ifndef NX_ASSERT_H
#define NX_ASSERT_H

#include <niu2x/log.h>
#include "niu2x/exception.h"

#define NX_ASSERT(condition, message, ...)                                     \
    if (!(condition)) {                                                        \
        NX_LOG_F(message, ##__VA_ARGS__);                                      \
        NX_THROW((message));                                                   \
    }

#define NX_ASSERT_NO_THROW(condition, message, ...)                            \
    if (!(condition)) {                                                        \
        NX_LOG_F(message, ##__VA_ARGS__);                                      \
    }

#endif