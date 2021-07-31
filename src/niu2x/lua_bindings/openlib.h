#ifndef NX_LB_OPEN_LIB_H
#define NX_LB_OPEN_LIB_H

extern "C" {

#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

#include <niu2x/utils.h>

namespace nx::lua_bindings {

void openlib_all(lua_State* L);
void openlib_log(lua_State* L);
void openlib_cvt(lua_State* L);
void openlib_hardcodelua(lua_State* L);

} // namespace nx::lua_bindings

#endif