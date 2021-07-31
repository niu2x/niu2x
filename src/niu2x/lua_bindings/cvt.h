#ifndef NX_LB_CVT_H
#define NX_LB_CVT_H

#include <niu2x/utils.h>

namespace nx::lua_bindings {

struct cvt_utils {
    static std::string hex_encode(const memref& mref);
    static std::string hex_decode(const memref& mref);
    static std::string zlib_compress(const memref& mref);
    static std::string zlib_uncompress(const memref& mref);
};

} // namespace nx::lua_bindings

#endif