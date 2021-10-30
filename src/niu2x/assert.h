#ifndef NX_ASSERT_H
#define NX_ASSERT_H

#include "exception.h"

#define NX_ASSERT(condition, message, ...)                                     \
    if (!(condition)) {                                                        \
        NX_LOG_F(message, ##__VA_ARGS__);                                      \
        NX_THROW((message));                                                   \
    }

#endif