#ifndef NX_LB_CVT_H
#define NX_LB_CVT_H

#include <niu2x/utils.h>

namespace nx::lua_bindings {

struct API cvt {
    static std::string hex_encode(const memref& mref);
    static std::string hex_decode(const memref& mref);
    static std::string zlib_compress(const memref& mref);
    static std::string zlib_uncompress(const memref& mref);

    static std::string hex_encode(const std::string& sz)
    {
        memref mref = { (void*)sz.c_str(), sz.length() };
        return hex_encode(mref);
    }

    static std::string hex_decode(const std::string& sz)
    {
        memref mref = { (void*)sz.c_str(), sz.length() };
        return hex_decode(mref);
    }

    static std::string zlib_compress(const std::string& sz)
    {
        memref mref = { (void*)sz.c_str(), sz.length() };
        return zlib_compress(mref);
    }

    static std::string zlib_uncompress(const std::string& sz)
    {
        memref mref = { (void*)sz.c_str(), sz.length() };
        return zlib_uncompress(mref);
    }
};

} // namespace nx::lua_bindings

#endif