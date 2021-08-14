#ifndef NXLUA_LUA_UTILS_H
#define NXLUA_LUA_UTILS_H

#include <sstream>

#include <niu2x/utils.h>

struct lua_State;

namespace nxlua {

struct lua_utils {
    static void dobuffer(lua_State* L, const nx::const_memref& mref);
    static void call(lua_State* L, int nargs, int nrets);
};

} // namespace nxlua

#define NXLUA_CHECK_LUA(exp)                                                   \
    {                                                                          \
        if ((exp) != LUA_OK) {                                                 \
            std::stringstream ss;                                              \
            ss << "Lua Error: ";                                               \
            ss << lua_tostring(L, -1);                                         \
            lua_pop(L, 1);                                                     \
            NX_THROW(ss.str().c_str());                                        \
        }                                                                      \
    }

#endif