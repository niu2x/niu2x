#ifndef NXLUA_TOLUA_FIX_H
#define NXLUA_TOLUA_FIX_H

#include <niu2x/utils.h>

struct lua_State;
struct tolua_Error;

extern void tolua_pushnxcppstring(lua_State* L, const std::string& sz);
extern std::string tolua_tonxcppstring(lua_State* L, int idx, int def);
extern int tolua_isnxcppstring(
    lua_State* L, int idx, int def, tolua_Error* err);

#endif