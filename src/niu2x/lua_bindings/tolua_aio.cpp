/*
** Lua binding: aio
** Generated automatically by tolua++-1.0.93 on Fri Aug  6 02:34:34 2021.
*/

#ifndef __cplusplus
    #include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int tolua_aio_open(lua_State* tolua_S);

#include <niu2x/lua_bindings/aio.h>
#include <niu2x/lua_bindings/tolua_fix.h>

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_nx__lua_bindings__aio__event_loop__rid(
    lua_State* tolua_S)
{
    nx::lua_bindings::aio::event_loop::rid* self
        = (nx::lua_bindings::aio::event_loop::rid*)tolua_tousertype(
            tolua_S, 1, 0);
    Mtolua_delete(self);
    return 0;
}

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
    tolua_usertype(tolua_S, "nx::lua_bindings::aio::status");
    tolua_usertype(tolua_S, "nx::lua_bindings::aio::event_loop");
    tolua_usertype(tolua_S, "nx::lua_bindings::aio::event_loop::rid");
}

/* get function: id of class  rid */
#ifndef TOLUA_DISABLE_tolua_get_nx__lua_bindings__aio__event_loop__rid_id
static int tolua_get_nx__lua_bindings__aio__event_loop__rid_id(
    lua_State* tolua_S)
{
    nx::lua_bindings::aio::event_loop::rid* self
        = (nx::lua_bindings::aio::event_loop::rid*)tolua_tousertype(
            tolua_S, 1, 0);
    #ifndef TOLUA_RELEASE
    if (!self)
        tolua_error(tolua_S, "invalid 'self' in accessing variable 'id'", NULL);
    #endif
    tolua_pushnumber(tolua_S, (lua_Number)self->id);
    return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: id of class  rid */
#ifndef TOLUA_DISABLE_tolua_set_nx__lua_bindings__aio__event_loop__rid_id
static int tolua_set_nx__lua_bindings__aio__event_loop__rid_id(
    lua_State* tolua_S)
{
    nx::lua_bindings::aio::event_loop::rid* self
        = (nx::lua_bindings::aio::event_loop::rid*)tolua_tousertype(
            tolua_S, 1, 0);
    #ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!self)
        tolua_error(tolua_S, "invalid 'self' in accessing variable 'id'", NULL);
    if (!tolua_isnumber(tolua_S, 2, 0, &tolua_err))
        tolua_error(
            tolua_S, "#vinvalid type in variable assignment.", &tolua_err);
    #endif
    self->id = ((uint16_t)tolua_tonumber(tolua_S, 2, 0));
    return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  rid */
#ifndef TOLUA_DISABLE_tolua_aio_nx_lua_bindings_aio_event_loop_rid_new00
static int tolua_aio_nx_lua_bindings_aio_event_loop_rid_new00(
    lua_State* tolua_S)
{
    #ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertable(
            tolua_S, 1, "nx::lua_bindings::aio::event_loop::rid", 0, &tolua_err)
        || !tolua_isnoobj(tolua_S, 2, &tolua_err))
        goto tolua_lerror;
    else
    #endif
    {
        {
            nx::lua_bindings::aio::event_loop::rid* tolua_ret
                = (nx::lua_bindings::aio::event_loop::rid*)Mtolua_new(
                    (nx::lua_bindings::aio::event_loop::rid)());
            tolua_pushusertype(tolua_S, (void*)tolua_ret,
                "nx::lua_bindings::aio::event_loop::rid");
        }
    }
    return 1;
    #ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "#ferror in function 'new'.", &tolua_err);
    return 0;
    #endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  rid */
#ifndef TOLUA_DISABLE_tolua_aio_nx_lua_bindings_aio_event_loop_rid_new00_local
static int tolua_aio_nx_lua_bindings_aio_event_loop_rid_new00_local(
    lua_State* tolua_S)
{
    #ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertable(
            tolua_S, 1, "nx::lua_bindings::aio::event_loop::rid", 0, &tolua_err)
        || !tolua_isnoobj(tolua_S, 2, &tolua_err))
        goto tolua_lerror;
    else
    #endif
    {
        {
            nx::lua_bindings::aio::event_loop::rid* tolua_ret
                = (nx::lua_bindings::aio::event_loop::rid*)Mtolua_new(
                    (nx::lua_bindings::aio::event_loop::rid)());
            tolua_pushusertype(tolua_S, (void*)tolua_ret,
                "nx::lua_bindings::aio::event_loop::rid");
            tolua_register_gc(tolua_S, lua_gettop(tolua_S));
        }
    }
    return 1;
    #ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "#ferror in function 'new'.", &tolua_err);
    return 0;
    #endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  rid */
#ifndef TOLUA_DISABLE_tolua_aio_nx_lua_bindings_aio_event_loop_rid_delete00
static int tolua_aio_nx_lua_bindings_aio_event_loop_rid_delete00(
    lua_State* tolua_S)
{
    #ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertype(
            tolua_S, 1, "nx::lua_bindings::aio::event_loop::rid", 0, &tolua_err)
        || !tolua_isnoobj(tolua_S, 2, &tolua_err))
        goto tolua_lerror;
    else
    #endif
    {
        nx::lua_bindings::aio::event_loop::rid* self
            = (nx::lua_bindings::aio::event_loop::rid*)tolua_tousertype(
                tolua_S, 1, 0);
    #ifndef TOLUA_RELEASE
        if (!self)
            tolua_error(tolua_S, "invalid 'self' in function 'delete'", NULL);
    #endif
        Mtolua_delete(self);
    }
    return 0;
    #ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "#ferror in function 'delete'.", &tolua_err);
    return 0;
    #endif
}
#endif //#ifndef TOLUA_DISABLE

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

/* method: run of class  nx::lua_bindings::aio::event_loop */
#ifndef TOLUA_DISABLE_tolua_aio_nx_lua_bindings_aio_event_loop_run00
static int tolua_aio_nx_lua_bindings_aio_event_loop_run00(lua_State* tolua_S)
{
    #ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertype(
            tolua_S, 1, "nx::lua_bindings::aio::event_loop", 0, &tolua_err)
        || !tolua_isnumber(tolua_S, 2, 0, &tolua_err)
        || !tolua_isnoobj(tolua_S, 3, &tolua_err))
        goto tolua_lerror;
    else
    #endif
    {
        nx::lua_bindings::aio::event_loop* self
            = (nx::lua_bindings::aio::event_loop*)tolua_tousertype(
                tolua_S, 1, 0);
        int p_type = ((int)tolua_tonumber(tolua_S, 2, 0));
    #ifndef TOLUA_RELEASE
        if (!self)
            tolua_error(tolua_S, "invalid 'self' in function 'run'", NULL);
    #endif
        {
            self->run(p_type);
        }
    }
    return 0;
    #ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "#ferror in function 'run'.", &tolua_err);
    return 0;
    #endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: run of class  nx::lua_bindings::aio::event_loop */
#ifndef TOLUA_DISABLE_tolua_aio_nx_lua_bindings_aio_event_loop_run01
static int tolua_aio_nx_lua_bindings_aio_event_loop_run01(lua_State* tolua_S)
{
    tolua_Error tolua_err;
    if (!tolua_isusertype(
            tolua_S, 1, "nx::lua_bindings::aio::event_loop", 0, &tolua_err)
        || !tolua_isnoobj(tolua_S, 2, &tolua_err))
        goto tolua_lerror;
    else {
        nx::lua_bindings::aio::event_loop* self
            = (nx::lua_bindings::aio::event_loop*)tolua_tousertype(
                tolua_S, 1, 0);
    #ifndef TOLUA_RELEASE
        if (!self)
            tolua_error(tolua_S, "invalid 'self' in function 'run'", NULL);
    #endif
        {
            self->run();
        }
    }
    return 0;
tolua_lerror:
    return tolua_aio_nx_lua_bindings_aio_event_loop_run00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: create_tcp of class  nx::lua_bindings::aio::event_loop */
#ifndef TOLUA_DISABLE_tolua_aio_nx_lua_bindings_aio_event_loop_create_tcp00
static int tolua_aio_nx_lua_bindings_aio_event_loop_create_tcp00(
    lua_State* tolua_S)
{
    #ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertype(
            tolua_S, 1, "nx::lua_bindings::aio::event_loop", 0, &tolua_err)
        || !tolua_isnoobj(tolua_S, 2, &tolua_err))
        goto tolua_lerror;
    else
    #endif
    {
        nx::lua_bindings::aio::event_loop* self
            = (nx::lua_bindings::aio::event_loop*)tolua_tousertype(
                tolua_S, 1, 0);
    #ifndef TOLUA_RELEASE
        if (!self)
            tolua_error(
                tolua_S, "invalid 'self' in function 'create_tcp'", NULL);
    #endif
        {
            nx::lua_bindings::aio::event_loop::rid tolua_ret
                = (nx::lua_bindings::aio::event_loop::rid)self->create_tcp();
            {
    #ifdef __cplusplus
                void* tolua_obj = Mtolua_new(
                    (nx::lua_bindings::aio::event_loop::rid)(tolua_ret));
                tolua_pushusertype(tolua_S, tolua_obj,
                    "nx::lua_bindings::aio::event_loop::rid");
                tolua_register_gc(tolua_S, lua_gettop(tolua_S));
    #else
                void* tolua_obj = tolua_copy(tolua_S, (void*)&tolua_ret,
                    sizeof(nx::lua_bindings::aio::event_loop::rid));
                tolua_pushusertype(tolua_S, tolua_obj,
                    "nx::lua_bindings::aio::event_loop::rid");
                tolua_register_gc(tolua_S, lua_gettop(tolua_S));
    #endif
            }
        }
    }
    return 1;
    #ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "#ferror in function 'create_tcp'.", &tolua_err);
    return 0;
    #endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destory_tcp of class  nx::lua_bindings::aio::event_loop */
#ifndef TOLUA_DISABLE_tolua_aio_nx_lua_bindings_aio_event_loop_destory_tcp00
static int tolua_aio_nx_lua_bindings_aio_event_loop_destory_tcp00(
    lua_State* tolua_S)
{
    #ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertype(
            tolua_S, 1, "nx::lua_bindings::aio::event_loop", 0, &tolua_err)
        || (tolua_isvaluenil(tolua_S, 2, &tolua_err)
            || !tolua_isusertype(tolua_S, 2,
                "nx::lua_bindings::aio::event_loop::rid", 0, &tolua_err))
        || !tolua_isnoobj(tolua_S, 3, &tolua_err))
        goto tolua_lerror;
    else
    #endif
    {
        nx::lua_bindings::aio::event_loop* self
            = (nx::lua_bindings::aio::event_loop*)tolua_tousertype(
                tolua_S, 1, 0);
        nx::lua_bindings::aio::event_loop::rid tcp_id
            = *((nx::lua_bindings::aio::event_loop::rid*)tolua_tousertype(
                tolua_S, 2, 0));
    #ifndef TOLUA_RELEASE
        if (!self)
            tolua_error(
                tolua_S, "invalid 'self' in function 'destory_tcp'", NULL);
    #endif
        {
            self->destory_tcp(tcp_id);
        }
    }
    return 0;
    #ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "#ferror in function 'destory_tcp'.", &tolua_err);
    return 0;
    #endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: connect of class  nx::lua_bindings::aio::event_loop */
#ifndef TOLUA_DISABLE_tolua_aio_nx_lua_bindings_aio_event_loop_connect00
static int tolua_aio_nx_lua_bindings_aio_event_loop_connect00(
    lua_State* tolua_S)
{
    #ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertype(
            tolua_S, 1, "nx::lua_bindings::aio::event_loop", 0, &tolua_err)
        || (tolua_isvaluenil(tolua_S, 2, &tolua_err)
            || !tolua_isusertype(tolua_S, 2,
                "nx::lua_bindings::aio::event_loop::rid", 0, &tolua_err))
        || !tolua_isstring(tolua_S, 3, 0, &tolua_err)
        || !tolua_isnumber(tolua_S, 4, 0, &tolua_err)
        || !tolua_isnx_luafunction(tolua_S, 5, 0, &tolua_err)
        || !tolua_isnoobj(tolua_S, 6, &tolua_err))
        goto tolua_lerror;
    else
    #endif
    {
        nx::lua_bindings::aio::event_loop* self
            = (nx::lua_bindings::aio::event_loop*)tolua_tousertype(
                tolua_S, 1, 0);
        nx::lua_bindings::aio::event_loop::rid tcp_id
            = *((nx::lua_bindings::aio::event_loop::rid*)tolua_tousertype(
                tolua_S, 2, 0));
        const char* address = ((const char*)tolua_tostring(tolua_S, 3, 0));
        uint16_t port = ((uint16_t)tolua_tonumber(tolua_S, 4, 0));
        nx_luafunction callback
            = ((nx_luafunction)tolua_tonx_luafunction(tolua_S, 5, 0));
    #ifndef TOLUA_RELEASE
        if (!self)
            tolua_error(tolua_S, "invalid 'self' in function 'connect'", NULL);
    #endif
        {
            self->connect(tcp_id, address, port, callback);
        }
    }
    return 0;
    #ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "#ferror in function 'connect'.", &tolua_err);
    return 0;
    #endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: start_read of class  nx::lua_bindings::aio::event_loop */
#ifndef TOLUA_DISABLE_tolua_aio_nx_lua_bindings_aio_event_loop_start_read00
static int tolua_aio_nx_lua_bindings_aio_event_loop_start_read00(
    lua_State* tolua_S)
{
    #ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertype(
            tolua_S, 1, "nx::lua_bindings::aio::event_loop", 0, &tolua_err)
        || (tolua_isvaluenil(tolua_S, 2, &tolua_err)
            || !tolua_isusertype(tolua_S, 2,
                "nx::lua_bindings::aio::event_loop::rid", 0, &tolua_err))
        || !tolua_isnx_luafunction(tolua_S, 3, 0, &tolua_err)
        || !tolua_isnoobj(tolua_S, 4, &tolua_err))
        goto tolua_lerror;
    else
    #endif
    {
        nx::lua_bindings::aio::event_loop* self
            = (nx::lua_bindings::aio::event_loop*)tolua_tousertype(
                tolua_S, 1, 0);
        nx::lua_bindings::aio::event_loop::rid stream_id
            = *((nx::lua_bindings::aio::event_loop::rid*)tolua_tousertype(
                tolua_S, 2, 0));
        nx_luafunction callback
            = ((nx_luafunction)tolua_tonx_luafunction(tolua_S, 3, 0));
    #ifndef TOLUA_RELEASE
        if (!self)
            tolua_error(
                tolua_S, "invalid 'self' in function 'start_read'", NULL);
    #endif
        {
            self->start_read(stream_id, callback);
        }
    }
    return 0;
    #ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "#ferror in function 'start_read'.", &tolua_err);
    return 0;
    #endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: stop_read of class  nx::lua_bindings::aio::event_loop */
#ifndef TOLUA_DISABLE_tolua_aio_nx_lua_bindings_aio_event_loop_stop_read00
static int tolua_aio_nx_lua_bindings_aio_event_loop_stop_read00(
    lua_State* tolua_S)
{
    #ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertype(
            tolua_S, 1, "nx::lua_bindings::aio::event_loop", 0, &tolua_err)
        || (tolua_isvaluenil(tolua_S, 2, &tolua_err)
            || !tolua_isusertype(tolua_S, 2,
                "nx::lua_bindings::aio::event_loop::rid", 0, &tolua_err))
        || !tolua_isnoobj(tolua_S, 3, &tolua_err))
        goto tolua_lerror;
    else
    #endif
    {
        nx::lua_bindings::aio::event_loop* self
            = (nx::lua_bindings::aio::event_loop*)tolua_tousertype(
                tolua_S, 1, 0);
        nx::lua_bindings::aio::event_loop::rid stream_id
            = *((nx::lua_bindings::aio::event_loop::rid*)tolua_tousertype(
                tolua_S, 2, 0));
    #ifndef TOLUA_RELEASE
        if (!self)
            tolua_error(
                tolua_S, "invalid 'self' in function 'stop_read'", NULL);
    #endif
        {
            self->stop_read(stream_id);
        }
    }
    return 0;
    #ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "#ferror in function 'stop_read'.", &tolua_err);
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
    tolua_cclass(tolua_S, "status", "nx::lua_bindings::aio::status", "", NULL);
    tolua_beginmodule(tolua_S, "status");
    tolua_constant(tolua_S, "ok", (int)nx::lua_bindings::aio::status::ok);
    tolua_constant(tolua_S, "fail", (int)nx::lua_bindings::aio::status::fail);
    tolua_endmodule(tolua_S);
#ifdef __cplusplus
    tolua_cclass(tolua_S, "event_loop", "nx::lua_bindings::aio::event_loop", "",
        tolua_collect_nx__lua_bindings__aio__event_loop);
#else
    tolua_cclass(
        tolua_S, "event_loop", "nx::lua_bindings::aio::event_loop", "", NULL);
#endif
    tolua_beginmodule(tolua_S, "event_loop");
#ifdef __cplusplus
    tolua_cclass(tolua_S, "rid", "nx::lua_bindings::aio::event_loop::rid", "",
        tolua_collect_nx__lua_bindings__aio__event_loop__rid);
#else
    tolua_cclass(
        tolua_S, "rid", "nx::lua_bindings::aio::event_loop::rid", "", NULL);
#endif
    tolua_beginmodule(tolua_S, "rid");
    tolua_variable(tolua_S, "id",
        tolua_get_nx__lua_bindings__aio__event_loop__rid_id,
        tolua_set_nx__lua_bindings__aio__event_loop__rid_id);
    tolua_function(
        tolua_S, "new", tolua_aio_nx_lua_bindings_aio_event_loop_rid_new00);
    tolua_function(tolua_S, "new_local",
        tolua_aio_nx_lua_bindings_aio_event_loop_rid_new00_local);
    tolua_function(tolua_S, ".call",
        tolua_aio_nx_lua_bindings_aio_event_loop_rid_new00_local);
    tolua_function(tolua_S, "delete",
        tolua_aio_nx_lua_bindings_aio_event_loop_rid_delete00);
    tolua_endmodule(tolua_S);
    tolua_function(
        tolua_S, "create", tolua_aio_nx_lua_bindings_aio_event_loop_create00);
    tolua_function(
        tolua_S, "run", tolua_aio_nx_lua_bindings_aio_event_loop_run00);
    tolua_function(
        tolua_S, "run", tolua_aio_nx_lua_bindings_aio_event_loop_run01);
    tolua_function(tolua_S, "create_tcp",
        tolua_aio_nx_lua_bindings_aio_event_loop_create_tcp00);
    tolua_function(tolua_S, "destory_tcp",
        tolua_aio_nx_lua_bindings_aio_event_loop_destory_tcp00);
    tolua_function(
        tolua_S, "connect", tolua_aio_nx_lua_bindings_aio_event_loop_connect00);
    tolua_function(tolua_S, "start_read",
        tolua_aio_nx_lua_bindings_aio_event_loop_start_read00);
    tolua_function(tolua_S, "stop_read",
        tolua_aio_nx_lua_bindings_aio_event_loop_stop_read00);
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
