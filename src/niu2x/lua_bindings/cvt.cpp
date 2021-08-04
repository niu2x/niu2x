#include "cvt.h"

#include <niu2x/io.h>

#define IMP(filter_name)                                                       \
    std::string cvt::filter_name(const memref& mref)                           \
    {                                                                          \
        std::stringstream device;                                              \
        io::sink::adapter<uint8_t, std::ostream> sink(device);                 \
        io::source::bytes source(                                              \
            reinterpret_cast<uint8_t*>(mref.base), mref.size);                 \
        io::pipe(source, io::filter::filter_name, sink);                       \
        return device.str();                                                   \
    }

namespace nx::lua_bindings {

IMP(hex_encode)
IMP(hex_decode)
IMP(zlib_compress)
IMP(zlib_uncompress)

} // namespace nx::lua_bindings