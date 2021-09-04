#include "tolua_fix.h"

#include <niu2x_lua/tolua/tolua++.h>
#include <niu2x_lua/lua_engine.h>
#include <niu2x_lua/lua_utils.h>

void tolua_pushnxcppstring(lua_State* L, const std::string& sz)
{
    lua_pushlstring(L, sz.c_str(), sz.length());
}

std::string tolua_tonxcppstring(lua_State* L, int idx, int def)
{
    nx::unused(def);

    if (lua_gettop(L) < abs(idx))
        return "";
    else {
        size_t len;
        auto* ptr = lua_tolstring(L, idx, &len);
        return std::string(ptr, len);
    }
}

int tolua_isnxcppstring(lua_State* L, int idx, int def, tolua_Error* err)
{
    if (def && lua_gettop(L) < abs(idx))
        return 1;
    if (lua_isnil(L, idx) || lua_isstring(L, idx))
        return 1;
    err->index = idx;
    err->array = 0;
    err->type = "string";
    return 0;
}

int tolua_isnx_luafunction(lua_State* L, int idx, int def, tolua_Error* err)
{
    if (def && lua_gettop(L) < abs(idx))
        return 1;

    if (lua_isfunction(L, idx))
        return 1;

    err->index = idx;
    err->array = 0;
    err->type = "nx_luafunction";
    return 0;
}

nx_luafunction tolua_tonx_luafunction(lua_State* L, int idx, int def)
{
    nx::unused(def);
    return nx_luafunction(L, idx);
}

nx_luafunction::luafunc_ref::luafunc_ref(lua_State* L, int idx)
: L_(L)
, func_id_(next_func_id_++)
{
    lua_getfield(L_, LUA_REGISTRYINDEX, "nx_luafunction");
    if (lua_isnil(L_, -1)) {
        lua_pop(L_, 1);
        lua_newtable(L_);
        lua_setfield(L_, LUA_REGISTRYINDEX, "nx_luafunction");
        lua_getfield(L_, LUA_REGISTRYINDEX, "nx_luafunction");
    }
    NX_ASSERT(lua_istable(L_, -1), "");
    lua_pushnumber(L_, func_id_);
    lua_pushvalue(L_, idx);
    lua_settable(L_, -3);
    lua_pop(L_, 1);
    // NX_LOG_D("ref func_id_ %d", func_id_);
}

nx_luafunction::luafunc_ref::~luafunc_ref()
{
    lua_getfield(L_, LUA_REGISTRYINDEX, "nx_luafunction");
    if (lua_istable(L_, -1)) {

        lua_pushnumber(L_, func_id_);
        lua_pushnil(L_);
        lua_settable(L_, -3);
        lua_pop(L_, 1);
    }
    // NX_LOG_D("unref func_id_ %d", func_id_);
}

uint32_t nx_luafunction::luafunc_ref::next_func_id_ = 0;

nx_luafunction::nx_luafunction(lua_State* L, int idx)

: ref_(std::make_shared<luafunc_ref>(L, idx))
{
}

nx_luafunction::~nx_luafunction() { }

void nx_luafunction::call(
    const std::function<int(struct lua_State*)>& callback) const
{
    ref_->call(callback);
}

void nx_luafunction::luafunc_ref::call(
    const std::function<int(struct lua_State*)>& callback) const
{
    lua_getfield(L_, LUA_REGISTRYINDEX, "nx_luafunction");
    lua_pushnumber(L_, func_id_);
    lua_gettable(L_, -2);

    // NX_LOG_D("call func_id_ %d", func_id_);

    NX_ASSERT(lua_isfunction(L_, -1), "");

    nxlua::lua_utils::call(L_, callback(L_), 0);

    lua_settop(L_, 0);
}
