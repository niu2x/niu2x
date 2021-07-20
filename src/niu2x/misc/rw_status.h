#ifndef NX_MISC_RW_STATUS_H
#define NX_MISC_RW_STATUS_H

namespace nx::misc {

enum class API rw_status {
    ok,
    again,
    eof,
};


}

#endif