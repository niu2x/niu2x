#ifndef NX_IO_H
#define NX_IO_H

#include <niu2x/utils.h>

namespace nx::io {

void API read_file(const char *pathname, std::vector<uint8_t> &output);

};

#endif