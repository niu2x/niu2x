#include <niu2x/lua_engine.h>

extern "C" {

#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

#include <niu2x/lua_bindings/openlib.h>

namespace nx {

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

lua_engine::lua_engine(size_t memory_limit)
: L(nullptr)
, memory_(memory_limit)
, allocator_(memory_)
{
    L = lua_newstate(&mem_alloc, &allocator_);
    if (L) {
        lua_atpanic(L, panic);
        luaL_openlibs(L);
        lua_bindings::openlib_all(L);
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
    auto* allocator = reinterpret_cast<mm::allocator*>(ud);
    if (nsize == 0) {
        if (ptr) {
            allocator->free(ptr);
        }
        return nullptr;
    } else {
        if (osize >= nsize) {
            return ptr;
        }
        auto* new_ptr = allocator->allocate(nsize, 8);
        if (new_ptr && ptr && osize) {
            memcpy(new_ptr, ptr, min(osize, nsize));
            allocator->free(ptr);
        }
        return new_ptr;
    }
}

void lua_engine::dostring(const char* code)
{
    const int ret = luaL_dostring(L, code);
    if (ret != LUA_OK) {
        std::stringstream ss;
        ss << "Lua Error: ";
        ss << lua_tostring(L, -1);
        lua_pop(L, 1);
        NX_THROW(ss.str().c_str());
    }
}

void lua_engine::dofile(const char* file)
{
    const int ret = luaL_dofile(L, file);
    if (ret != LUA_OK) {
        std::stringstream ss;
        ss << "Lua Error: ";
        ss << lua_tostring(L, -1);
        lua_pop(L, 1);

        NX_THROW(ss.str().c_str());
    }
}

} // namespace nx
