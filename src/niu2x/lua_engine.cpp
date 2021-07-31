#include <niu2x/lua_engine.h>

extern "C" {

#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

#include <niu2x/lua_bindings/openlib.h>

#define CHECK(exp)                                                             \
    {                                                                          \
        if ((exp) != LUA_OK) {                                                 \
            std::stringstream ss;                                              \
            ss << "Lua Error: ";                                               \
            ss << lua_tostring(L, -1);                                         \
            lua_pop(L, 1);                                                     \
            NX_THROW(ss.str().c_str());                                        \
        }                                                                      \
    }

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

#define lua_dobuffer(L, base, len)                                             \
    luaL_loadbuffer(L, ((const char*)base), len, "builtin")                    \
        || lua_pcall(L, 0, LUA_MULTRET, 0)

void lua_utils::dobuffer(lua_State* L, const memref& mref)
{
    CHECK(lua_dobuffer(L, mref.base, mref.size));
}

void lua_engine::dostring(const char* code) { CHECK(luaL_dostring(L, code)); }

void lua_engine::dofile(const char* file) { CHECK(luaL_dofile(L, file)); }

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
    CHECK(luaL_loadstring(L, code));
    lua_pushboolean(L, 1);
    CHECK(lua_pcall(L, 2, 1, 0));

    size_t n;
    const char* bytecode = lua_tolstring(L, -1, &n);

    output.insert(output.end(), bytecode, bytecode + n);

    lua_pop(L, 1);
}

} // namespace nx
