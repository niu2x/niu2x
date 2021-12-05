#include <niu2x/fs.h>

#include <fstream>

#include "niu2x/assert.h"

namespace nx::fs {

std::vector<uint8_t> read_file(const char* pathname)
{
    std::ifstream bin_file(pathname, std::ios::binary);
    NX_ASSERT(bin_file.good(), "read_file failed: %s", pathname);

    std::vector<uint8_t> data((std::istreambuf_iterator<char>(bin_file)),
        (std::istreambuf_iterator<char>()));

    bin_file.close();
    return data;
}

} // namespace nx::fs