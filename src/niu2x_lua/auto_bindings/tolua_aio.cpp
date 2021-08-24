/*
** Lua binding: aio
** Generated automatically by tolua++-1.0.93 on Wed Aug 25 01:25:05 2021.
*/

#ifndef __cplusplus
    #include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int tolua_aio_open(lua_State* tolua_S);

#include <niu2x_lua/bindings/aio.h>
using namespace nxlua;

/* function to release collected object via destructor */
#ifdef __cplusplus

// static int tolua_collect_nx__aio__rid(lua_State* tolua_S)
// {
//     nx::aio::rid* self = (nx::aio::rid*)tolua_tousertype(tolua_S, 1, 0);
//     Mtolua_delete(self);
//     return 0;
// }
#endif

/* function to register type */
static void tolua_reg_types(lua_State* tolua_S)
{
    tolua_usertype(tolua_S, "nx::aio::rid");
}

/* function: aio::run */
#ifndef TOLUA_DISABLE_tolua_aio_aio_run00
static int tolua_aio_aio_run00(lua_State* tolua_S)
{
    #ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isnumber(tolua_S, 1, 1, &tolua_err)
        || !tolua_isnoobj(tolua_S, 2, &tolua_err))
        goto tolua_lerror;
    else
    #endif
    {
        int t = ((int)tolua_tonumber(tolua_S, 1, aio::wait_all));
        {
            aio::run(t);
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

/* function: aio::create_idle */
#ifndef TOLUA_DISABLE_tolua_aio_aio_create_idle00
static int tolua_aio_aio_create_idle00(lua_State* tolua_S)
{
    #ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isnx_luafunction(tolua_S, 1, 0, &tolua_err)
        || !tolua_isnoobj(tolua_S, 2, &tolua_err))
        goto tolua_lerror;
    else
    #endif
    {
        nx_luafunction tolua_var_1
            = ((nx_luafunction)tolua_tonx_luafunction(tolua_S, 1, 0));
        {
            nx::aio::rid tolua_ret
                = (nx::aio::rid)aio::create_idle(tolua_var_1);
            {
    #ifdef __cplusplus
                void* tolua_obj = Mtolua_new((nx::aio::rid)(tolua_ret));
                tolua_pushusertype(tolua_S, tolua_obj, "nx::aio::rid");
                tolua_register_gc(tolua_S, lua_gettop(tolua_S));
    #else
                void* tolua_obj = tolua_copy(
                    tolua_S, (void*)&tolua_ret, sizeof(nx::aio::rid));
                tolua_pushusertype(tolua_S, tolua_obj, "nx::aio::rid");
                tolua_register_gc(tolua_S, lua_gettop(tolua_S));
    #endif
            }
        }
    }
    return 1;
    #ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "#ferror in function 'create_idle'.", &tolua_err);
    return 0;
    #endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: aio::destroy_idle */
#ifndef TOLUA_DISABLE_tolua_aio_aio_destroy_idle00
static int tolua_aio_aio_destroy_idle00(lua_State* tolua_S)
{
    #ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if ((tolua_isvaluenil(tolua_S, 1, &tolua_err)
            || !tolua_isusertype(tolua_S, 1, "nx::aio::rid", 0, &tolua_err))
        || !tolua_isnoobj(tolua_S, 2, &tolua_err))
        goto tolua_lerror;
    else
    #endif
    {
        nx::aio::rid idle = *((nx::aio::rid*)tolua_tousertype(tolua_S, 1, 0));
        {
            aio::destroy_idle(idle);
        }
    }
    return 0;
    #ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "#ferror in function 'destroy_idle'.", &tolua_err);
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
    tolua_module(tolua_S, "aio", 0);
    tolua_beginmodule(tolua_S, "aio");
    tolua_constant(tolua_S, "wait_all", aio::wait_all);
    tolua_constant(tolua_S, "wait_once", aio::wait_once);
    tolua_constant(tolua_S, "no_wait", aio::no_wait);
    tolua_function(tolua_S, "run", tolua_aio_aio_run00);
    tolua_function(tolua_S, "create_idle", tolua_aio_aio_create_idle00);
    tolua_function(tolua_S, "destroy_idle", tolua_aio_aio_destroy_idle00);
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
