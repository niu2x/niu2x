#include "tolua_fix.h"

#include <niu2x_lua/tolua/tolua++.h>
#include <niu2x_lua/lua_engine.h>

void tolua_pushnxcppstring(lua_State* L, const std::string& sz)
{
    lua_pushlstring(L, sz.c_str(), sz.length());
}

std::string tolua_tonxcppstring(lua_State* L, int idx, int def)
{
    nx::unused(def);

    if (lua_gettop(L) < abs(idx))
        return "";
    else {
        size_t len;
        auto* ptr = lua_tolstring(L, idx, &len);
        return std::string(ptr, len);
    }
}

int tolua_isnxcppstring(lua_State* L, int idx, int def, tolua_Error* err)
{
    if (def && lua_gettop(L) < abs(idx))
        return 1;
    if (lua_isnil(L, idx) || lua_isstring(L, idx))
        return 1;
    err->index = idx;
    err->array = 0;
    err->type = "string";
    return 0;
}
