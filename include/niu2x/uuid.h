#ifndef NX_UUID_H
#define NX_UUID_H

#include <string>
// #include <boost/uuid/uuid.hpp>
#include <niu2x/api.h>

namespace nx {

using uuid_t = struct {
    uint8_t bits[16];
};
NXAPI uuid_t uuid_gen();
NXAPI std::string uuid_str_gen();
NXAPI std::string uuid_str(const uuid_t* self);

} // namespace nx

#endif