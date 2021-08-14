#ifndef NX_UTILS_ASSERT_H
#define NX_UTILS_ASSERT_H

#include <niu2x/utils/exception.h>

#define NX_ASSERT(condition, message)                                          \
    if (!(condition)) {                                                        \
        NX_THROW((message));                                                   \
    }

#endif