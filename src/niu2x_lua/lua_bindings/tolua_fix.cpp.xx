#include "tolua_fix.h"

#include <niu2x/tolua/tolua++.h>
#include <niu2x/lua_engine.h>
#include <niu2x/lua_bindings/aio.h>

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

nx_luafunction tolua_tonx_luafunction(lua_State* L, int idx, int def)
{
    nx::unused(def);
    return nx_luafunction(L, idx);
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
}

void nx_luafunction::luafunc_ref::operator()(
    nx::aio::status status, nx::misc::rid rid)
{
    lua_getfield(L_, LUA_REGISTRYINDEX, "nx_luafunction");
    lua_pushnumber(L_, func_id_);
    lua_gettable(L_, -2);

    NX_ASSERT(lua_isfunction(L_, -1), "");

    lua_pushnumber(L_, (int)(status));

    void* tolua_obj = Mtolua_new((nx::lua_bindings::aio::event_loop::rid)(rid));
    tolua_pushusertype(L_, tolua_obj, "nx::lua_bindings::aio::event_loop::rid");
    tolua_register_gc(L_, lua_gettop(L_));

    nx::lua_utils::call(L_, 2, 0);

    lua_pop(L_, 1);
}

void nx_luafunction::luafunc_ref::operator()(
    nx::aio::status status, nx::misc::rid rid, const uint8_t* data, size_t len)
{
    lua_getfield(L_, LUA_REGISTRYINDEX, "nx_luafunction");
    lua_pushnumber(L_, func_id_);
    lua_gettable(L_, -2);

    NX_ASSERT(lua_isfunction(L_, -1), "");

    lua_pushnumber(L_, (int)(status));

    void* tolua_obj = Mtolua_new((nx::lua_bindings::aio::event_loop::rid)(rid));
    tolua_pushusertype(L_, tolua_obj, "nx::lua_bindings::aio::event_loop::rid");
    tolua_register_gc(L_, lua_gettop(L_));

    lua_pushlstring(L_, (const char*)data, len);

    nx::lua_utils::call(L_, 3, 0);

    lua_pop(L_, 1);
}

uint32_t nx_luafunction::luafunc_ref::next_func_id_ = 0;

nx_luafunction::nx_luafunction(lua_State* L, int idx)

: ref_(std::make_shared<luafunc_ref>(L, idx))
{
}

nx_luafunction::~nx_luafunction() { }