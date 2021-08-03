/*
** Lua binding: aio
** Generated automatically by tolua++-1.0.93 on Wed Aug  4 01:35:36 2021.
*/

#ifndef __cplusplus
    #include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

#define my_pushcppstring(L, sz) lua_pushlstring(L, sz.c_str(), sz.length())
/* Exported function */
TOLUA_API int tolua_aio_open(lua_State* tolua_S);

#include <niu2x/lua_bindings/aio.h>

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_nx__lua_bindings__aio__event_loop(lua_State* tolua_S)
{
    nx::lua_bindings::aio::event_loop* self
        = (nx::lua_bindings::aio::event_loop*)tolua_tousertype(tolua_S, 1, 0);
    Mtolua_delete(self);
    return 0;
}
#endif

/* function to register type */
static void tolua_reg_types(lua_State* tolua_S)
{
    tolua_usertype(tolua_S, "nx::lua_bindings::aio::event_loop");
}

/* method: create of class  nx::lua_bindings::aio::event_loop */
#ifndef TOLUA_DISABLE_tolua_aio_nx_lua_bindings_aio_event_loop_create00
static int tolua_aio_nx_lua_bindings_aio_event_loop_create00(lua_State* tolua_S)
{
    #ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertable(
            tolua_S, 1, "nx::lua_bindings::aio::event_loop", 0, &tolua_err)
        || !tolua_isnoobj(tolua_S, 2, &tolua_err))
        goto tolua_lerror;
    else
    #endif
    {
        {
            nx::lua_bindings::aio::event_loop tolua_ret
                = (nx::lua_bindings::aio::event_loop)
                    nx::lua_bindings::aio::event_loop::create();
            {
    #ifdef __cplusplus
                void* tolua_obj = Mtolua_new(
                    (nx::lua_bindings::aio::event_loop)(tolua_ret));
                tolua_pushusertype(
                    tolua_S, tolua_obj, "nx::lua_bindings::aio::event_loop");
                tolua_register_gc(tolua_S, lua_gettop(tolua_S));
    #else
                void* tolua_obj = tolua_copy(tolua_S, (void*)&tolua_ret,
                    sizeof(nx::lua_bindings::aio::event_loop));
                tolua_pushusertype(
                    tolua_S, tolua_obj, "nx::lua_bindings::aio::event_loop");
                tolua_register_gc(tolua_S, lua_gettop(tolua_S));
    #endif
            }
        }
    }
    return 1;
    #ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "#ferror in function 'create'.", &tolua_err);
    return 0;
    #endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_aio_open(lua_State* tolua_S)
{
    tolua_open(tolua_S);
    tolua_reg_types(tolua_S);
    tolua_module(tolua_S, NULL, 0);
    tolua_beginmodule(tolua_S, NULL);
    tolua_module(tolua_S, "nx", 0);
    tolua_beginmodule(tolua_S, "nx");
    tolua_module(tolua_S, "lua_bindings", 0);
    tolua_beginmodule(tolua_S, "lua_bindings");
    tolua_module(tolua_S, "aio", 0);
    tolua_beginmodule(tolua_S, "aio");
#ifdef __cplusplus
    tolua_cclass(tolua_S, "event_loop", "nx::lua_bindings::aio::event_loop", "",
        tolua_collect_nx__lua_bindings__aio__event_loop);
#else
    tolua_cclass(
        tolua_S, "event_loop", "nx::lua_bindings::aio::event_loop", "", NULL);
#endif
    tolua_beginmodule(tolua_S, "event_loop");
    tolua_function(
        tolua_S, "create", tolua_aio_nx_lua_bindings_aio_event_loop_create00);
    tolua_endmodule(tolua_S);
    tolua_endmodule(tolua_S);
    tolua_endmodule(tolua_S);
    tolua_endmodule(tolua_S);
    tolua_endmodule(tolua_S);
    return 1;
}

#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
TOLUA_API int luaopen_aio(lua_State* tolua_S)
{
    return tolua_aio_open(tolua_S);
}
#endif
