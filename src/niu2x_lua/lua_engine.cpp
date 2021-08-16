#include "lua_engine.h"

#include <string.h>

extern "C" {

#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

#include "tolua/tolua++.h"

#include "bindings.h"
#include "hardcode/lua_utils.h"
#include "auto_bindings/tolua_pipe.h"

namespace nxlua {

static int panic(lua_State* L)
{
    const char* s = lua_tostring(L, -1);
    std::stringstream ss;
    ss << "Lua Panic:(";
    ss << (s ? s : "?");
    ss << ")";
    NX_THROW(ss.str().c_str());
    return 0;
}

lua_engine::lua_engine(nx::memory_proxy allocator)
: L(nullptr)
, allocator_(allocator)
{
    L = lua_newstate(&mem_alloc, &allocator_);
    if (L) {
        lua_atpanic(L, panic);
        luaL_openlibs(L);
        bindings::openlib_log(L);
        tolua_pipe_open(L);

        lua_utils::dobuffer(L, hardcode::lua_utils_mref);
    }
}

lua_engine::~lua_engine()
{
    if (L) {
        lua_close(L);
    }
}

void* lua_engine::mem_alloc(void* ud, void* ptr, size_t osize, size_t nsize)
{
    auto* allocator = reinterpret_cast<nx::memory_proxy*>(ud);
    if (nsize == 0) {
        if (ptr) {
            allocator->free(ptr);
        }
        return nullptr;
    } else {
        if (osize >= nsize) {
            return ptr;
        }
        auto* new_ptr = allocator->allocate(nsize);
        if (new_ptr && ptr && osize) {
            memcpy(new_ptr, ptr, NX_MIN(osize, nsize));
            allocator->free(ptr);
        }
        return new_ptr;
    }
}

void lua_engine::dostring(const char* code)
{
    NXLUA_CHECK_LUA(luaL_dostring(L, code));
}

void lua_engine::dofile(const char* file)
{
    NXLUA_CHECK_LUA(luaL_dofile(L, file));
}

// static int my_writer (lua_State *L, const void* p, size_t sz, void* ud) {
//     unused(L);
//     auto *output = reinterpret_cast<std::vector<uint8_t> *>(ud);
//     auto *base = reinterpret_cast<const uint8_t *>(p);
//     output->insert(output->end(), base, base + sz);
//     return 0;
// }

void lua_engine::compile(const char* code, std::vector<uint8_t>& output)
{

    lua_getfield(L, LUA_GLOBALSINDEX, "string");
    lua_getfield(L, -1, "dump");
    NXLUA_CHECK_LUA(luaL_loadstring(L, code));
    lua_pushboolean(L, 1);
    NXLUA_CHECK_LUA(lua_pcall(L, 2, 1, 0));

    size_t n;
    const char* bytecode = lua_tolstring(L, -1, &n);

    output.insert(output.end(), bytecode, bytecode + n);

    lua_pop(L, 1);
}

void lua_engine::set_global_variable(
    const char* name, const char* const sz_list[])
{
    auto tbl = LUA_GLOBALSINDEX;
    const char* field = name;
    const char* dot;

    constexpr auto max_varname = nx::limits::max_varname;
    char tmpname[max_varname];

    while ((dot = strchr(field, '.')) != nullptr) {
        strncpy(tmpname, field, NX_MIN((size_t)(dot - field), max_varname - 1));
        tmpname[NX_MIN((size_t)(dot - field), max_varname - 1)] = 0;
        lua_getfield(L, tbl, tmpname);
        tbl = lua_gettop(L);
        field = dot + 1;
    }

    strncpy(tmpname, field, max_varname - 1);
    tmpname[max_varname - 1] = 0;
    lua_getfield(L, tbl, tmpname);
    tbl = lua_gettop(L);

    int idx = 0;
    while (sz_list[idx]) {
        lua_pushnumber(L, idx + 1);
        lua_pushstring(L, sz_list[idx++]);
        lua_settable(L, tbl);
    }

    if (lua_gettop(L)) {
        lua_pop(L, lua_gettop(L));
    }
}

} // namespace nxlua
