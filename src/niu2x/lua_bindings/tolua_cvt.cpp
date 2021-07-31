/*
** Lua binding: cvt
** Generated automatically by tolua++-1.0.93 on Sat Jul 31 17:55:21 2021.
*/

#ifndef __cplusplus
    #include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int tolua_cvt_open(lua_State* tolua_S);

#include <niu2x/utils.h>
#include <niu2x/lua_bindings/cvt.h>

/* function to register type */
static void tolua_reg_types(lua_State* tolua_S)
{
    tolua_usertype(tolua_S, "memref");
    tolua_usertype(tolua_S, "nx::lua_bindings::cvt_utils");
}

/* method: hex_encode of class  nx::lua_bindings::cvt_utils */
#ifndef TOLUA_DISABLE_tolua_cvt_nx_lua_bindings_cvt_utils_hex_encode00
static int tolua_cvt_nx_lua_bindings_cvt_utils_hex_encode00(lua_State* tolua_S)
{
    #ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertable(
            tolua_S, 1, "nx::lua_bindings::cvt_utils", 0, &tolua_err)
        || (tolua_isvaluenil(tolua_S, 2, &tolua_err)
            || !tolua_isusertype(tolua_S, 2, "const memref", 0, &tolua_err))
        || !tolua_isnoobj(tolua_S, 3, &tolua_err))
        goto tolua_lerror;
    else
    #endif
    {
        const memref* mref = ((const memref*)tolua_tousertype(tolua_S, 2, 0));
        {
            std::string tolua_ret
                = (std::string)nx::lua_bindings::cvt_utils::hex_encode(*mref);
            tolua_pushcppstring(tolua_S, (const char*)tolua_ret);
        }
    }
    return 1;
    #ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "#ferror in function 'hex_encode'.", &tolua_err);
    return 0;
    #endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hex_decode of class  nx::lua_bindings::cvt_utils */
#ifndef TOLUA_DISABLE_tolua_cvt_nx_lua_bindings_cvt_utils_hex_decode00
static int tolua_cvt_nx_lua_bindings_cvt_utils_hex_decode00(lua_State* tolua_S)
{
    #ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertable(
            tolua_S, 1, "nx::lua_bindings::cvt_utils", 0, &tolua_err)
        || (tolua_isvaluenil(tolua_S, 2, &tolua_err)
            || !tolua_isusertype(tolua_S, 2, "const memref", 0, &tolua_err))
        || !tolua_isnoobj(tolua_S, 3, &tolua_err))
        goto tolua_lerror;
    else
    #endif
    {
        const memref* mref = ((const memref*)tolua_tousertype(tolua_S, 2, 0));
        {
            std::string tolua_ret
                = (std::string)nx::lua_bindings::cvt_utils::hex_decode(*mref);
            tolua_pushcppstring(tolua_S, (const char*)tolua_ret);
        }
    }
    return 1;
    #ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "#ferror in function 'hex_decode'.", &tolua_err);
    return 0;
    #endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: zlib_compress of class  nx::lua_bindings::cvt_utils */
#ifndef TOLUA_DISABLE_tolua_cvt_nx_lua_bindings_cvt_utils_zlib_compress00
static int tolua_cvt_nx_lua_bindings_cvt_utils_zlib_compress00(
    lua_State* tolua_S)
{
    #ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertable(
            tolua_S, 1, "nx::lua_bindings::cvt_utils", 0, &tolua_err)
        || (tolua_isvaluenil(tolua_S, 2, &tolua_err)
            || !tolua_isusertype(tolua_S, 2, "const memref", 0, &tolua_err))
        || !tolua_isnoobj(tolua_S, 3, &tolua_err))
        goto tolua_lerror;
    else
    #endif
    {
        const memref* mref = ((const memref*)tolua_tousertype(tolua_S, 2, 0));
        {
            std::string tolua_ret
                = (std::string)nx::lua_bindings::cvt_utils::zlib_compress(
                    *mref);
            tolua_pushcppstring(tolua_S, (const char*)tolua_ret);
        }
    }
    return 1;
    #ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "#ferror in function 'zlib_compress'.", &tolua_err);
    return 0;
    #endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: zlib_uncompress of class  nx::lua_bindings::cvt_utils */
#ifndef TOLUA_DISABLE_tolua_cvt_nx_lua_bindings_cvt_utils_zlib_uncompress00
static int tolua_cvt_nx_lua_bindings_cvt_utils_zlib_uncompress00(
    lua_State* tolua_S)
{
    #ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertable(
            tolua_S, 1, "nx::lua_bindings::cvt_utils", 0, &tolua_err)
        || (tolua_isvaluenil(tolua_S, 2, &tolua_err)
            || !tolua_isusertype(tolua_S, 2, "const memref", 0, &tolua_err))
        || !tolua_isnoobj(tolua_S, 3, &tolua_err))
        goto tolua_lerror;
    else
    #endif
    {
        const memref* mref = ((const memref*)tolua_tousertype(tolua_S, 2, 0));
        {
            std::string tolua_ret
                = (std::string)nx::lua_bindings::cvt_utils::zlib_uncompress(
                    *mref);
            tolua_pushcppstring(tolua_S, (const char*)tolua_ret);
        }
    }
    return 1;
    #ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "#ferror in function 'zlib_uncompress'.", &tolua_err);
    return 0;
    #endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_cvt_open(lua_State* tolua_S)
{
    tolua_open(tolua_S);
    tolua_reg_types(tolua_S);
    tolua_module(tolua_S, NULL, 0);
    tolua_beginmodule(tolua_S, NULL);
    tolua_module(tolua_S, "nx", 0);
    tolua_beginmodule(tolua_S, "nx");
    tolua_module(tolua_S, "lua_bindings", 0);
    tolua_beginmodule(tolua_S, "lua_bindings");
    tolua_cclass(tolua_S, "cvt_utils", "nx::lua_bindings::cvt_utils", "", NULL);
    tolua_beginmodule(tolua_S, "cvt_utils");
    tolua_function(tolua_S, "hex_encode",
        tolua_cvt_nx_lua_bindings_cvt_utils_hex_encode00);
    tolua_function(tolua_S, "hex_decode",
        tolua_cvt_nx_lua_bindings_cvt_utils_hex_decode00);
    tolua_function(tolua_S, "zlib_compress",
        tolua_cvt_nx_lua_bindings_cvt_utils_zlib_compress00);
    tolua_function(tolua_S, "zlib_uncompress",
        tolua_cvt_nx_lua_bindings_cvt_utils_zlib_uncompress00);
    tolua_endmodule(tolua_S);
    tolua_endmodule(tolua_S);
    tolua_endmodule(tolua_S);
    tolua_endmodule(tolua_S);
    return 1;
}

#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
TOLUA_API int luaopen_cvt(lua_State* tolua_S)
{
    return tolua_cvt_open(tolua_S);
}
#endif
