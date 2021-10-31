#ifndef NX_SRC_IO_IO_H
#define NX_SRC_IO_IO_H

#include <niu2x/io.h>

#include "niu2x/utils.h"

namespace nx::io {

template <class Device>
int write(Device device, const void* data, size_t bytes);

template <class Device>
int read(Device device, void* data, size_t bytes);

template <>
int read(std::istream* device, void* data, size_t bytes);

template <>
int write(std::ostream* device, const void* data, size_t bytes);

}; // namespace nx::io

#endif