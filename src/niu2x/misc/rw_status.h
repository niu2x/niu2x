#ifndef NX_MISC_RW_STATUS_H
#define NX_MISC_RW_STATUS_H

#include <niu2x/api.h>

namespace nx::misc {

enum class API rw_status {
    ok,
    again,
    eof,
};


}

#endif