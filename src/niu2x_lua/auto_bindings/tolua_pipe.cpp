/*
** Lua binding: pipe
** Generated automatically by tolua++-1.0.93 on Tue Aug 17 00:28:26 2021.
*/

#ifndef __cplusplus
    #include "stdlib.h"
#endif
#include "string.h"

#include <niu2x_lua/tolua/tolua++.h>

/* Exported function */
TOLUA_API int tolua_pipe_open(lua_State* tolua_S);

#include <string>
#include <niu2x_lua/tolua_fix.h>
#include <niu2x_lua/bindings/pipe.h>
using namespace nxlua;

/* function to register type */
static void tolua_reg_types(lua_State* tolua_S)
{
    tolua_usertype(tolua_S, "filter");
}

/* function: pipe */
#ifndef TOLUA_DISABLE_tolua_pipe_pipe00
static int tolua_pipe_pipe00(lua_State* tolua_S)
{
    #ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isnxcppstring(tolua_S, 1, 0, &tolua_err)
        || !tolua_isnumber(tolua_S, 2, 0, &tolua_err)
        || !tolua_isnoobj(tolua_S, 3, &tolua_err))
        goto tolua_lerror;
    else
    #endif
    {
        const std::string src
            = ((const std::string)tolua_tonxcppstring(tolua_S, 1, 0));
        int tolua_var_1 = ((int)tolua_tonumber(tolua_S, 2, 0));
        {
            std::string tolua_ret = (std::string)pipe(src, tolua_var_1);
            tolua_pushnxcppstring(tolua_S, (std::string)tolua_ret);
            tolua_pushnxcppstring(tolua_S, (std::string)src);
        }
    }
    return 2;
    #ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "#ferror in function 'pipe'.", &tolua_err);
    return 0;
    #endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_pipe_open(lua_State* tolua_S)
{
    tolua_open(tolua_S);
    tolua_reg_types(tolua_S);
    tolua_module(tolua_S, NULL, 0);
    tolua_beginmodule(tolua_S, NULL);
    tolua_cclass(tolua_S, "filter", "filter", "", NULL);
    tolua_beginmodule(tolua_S, "filter");
    tolua_constant(tolua_S, "hex", filter::hex);
    tolua_constant(tolua_S, "unhex", filter::unhex);
    tolua_constant(tolua_S, "zlib", filter::zlib);
    tolua_constant(tolua_S, "unzlib", filter::unzlib);
    tolua_endmodule(tolua_S);
    tolua_function(tolua_S, "pipe", tolua_pipe_pipe00);
    tolua_endmodule(tolua_S);
    return 1;
}

#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
TOLUA_API int luaopen_pipe(lua_State* tolua_S)
{
    return tolua_pipe_open(tolua_S);
}
#endif
