#include "aio.h"
#include "niu2x_lua/tolua/tolua++.h"

namespace nxlua::aio {

void run(int type) { nx::aio::run(type); }

rid create_idle(nx_luafunction callback)
{
    return nx::aio::create_idle([callback](auto id) {
        callback.call([&id](struct lua_State* tolua_S) {
            void* tolua_obj = Mtolua_new((nx::aio::rid)(id));
            tolua_pushusertype(tolua_S, tolua_obj, "nx::aio::rid");
            tolua_register_gc(tolua_S, lua_gettop(tolua_S));

            return 1;
        });
    });
}
void destroy_idle(rid idle) { nx::aio::destroy_idle(idle); }

void openlib_aio(lua_State* L);

} // namespace nxlua::aio