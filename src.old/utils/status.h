#ifndef NX_UTILS_STATUS_H
#define NX_UTILS_STATUS_H

#include <niu2x/api.h>

namespace nx {

enum API status {
    ok,
    again,
    eof,
    fail,
};

} // namespace nx

#endif