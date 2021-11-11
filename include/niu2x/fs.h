#ifndef NX_FS_H
#define NX_FS_H

#include <vector>
#include <cstdint>

#include <niu2x/api.h>

namespace nx::fs {

NXAPI std::vector<uint8_t> read_file(const char* pathname);

};

#endif