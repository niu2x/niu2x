#include "openlib.h"

#include <niu2x/tolua/tolua++.h>

#include "tolua_cvt.h"
#include <niu2x/hardcode/lua_utils.h>
#include <niu2x/hardcode/lua_export_luabindings.h>
#include <niu2x/lua_engine.h>

namespace nx::lua_bindings {

void openlib_all(lua_State* L)
{
    openlib_log(L);
    openlib_cvt(L);
    openlib_hardcodelua(L);
}

void openlib_cvt(lua_State* L) { tolua_cvt_open(L); }

#define EXEC_EMBED_LUA(name)                                                   \
    lua_utils::dobuffer(L, hardcode::lua_##name##_mref);

void openlib_hardcodelua(lua_State* L)
{
    EXEC_EMBED_LUA(export_luabindings);
    EXEC_EMBED_LUA(utils);
}

} // namespace nx::lua_bindings