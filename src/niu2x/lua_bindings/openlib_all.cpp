#include "openlib.h"

#include <niu2x/tolua/tolua++.h>

#include "tolua_cvt.h"

namespace nx::lua_bindings {

void openlib_all(lua_State* L)
{
    openlib_log(L);
    openlib_cvt(L);
}

void openlib_cvt(lua_State* L) { tolua_cvt_open(L); }

} // namespace nx::lua_bindings