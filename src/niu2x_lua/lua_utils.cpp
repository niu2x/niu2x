#include "lua_utils.h"

extern "C" {

#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

#define lua_dobuffer(L, base, len)                                             \
    luaL_loadbuffer(L, ((const char*)base), len, "builtin")                    \
        || lua_pcall(L, 0, LUA_MULTRET, 0)

namespace nxlua {

void lua_utils::dobuffer(lua_State* L, const nx::memref& mref)
{
    NXLUA_CHECK_LUA(lua_dobuffer(L, mref.base, mref.size));
}

void lua_utils::call(lua_State* L, int nargs, int nrets)
{
    NXLUA_CHECK_LUA(lua_pcall(L, nargs, nrets, 0));
}

} // namespace nxlua