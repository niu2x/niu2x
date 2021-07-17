#include <niu2x/lua_engine.h>

extern "C" {

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

namespace niu2x {

static int panic(lua_State *L) {
    const char *s = lua_tostring(L, -1);
    fputs("PANIC: unprotected error in call to Lua API (", stderr);
    fputs(s ? s : "?", stderr);
    fputc(')', stderr);
    fputc('\n', stderr);
    fflush(stderr);
    return 0;
}

lua_engine::lua_engine(size_t memory_limit)
: L(nullptr)
, memory_(memory_limit)
, allocator_(memory_) {

    L = lua_newstate(&mem_alloc, &allocator_);
    if (L) {
        lua_atpanic(L, panic);
        luaL_openlibs(L);
    }
}

lua_engine::~lua_engine() {
    if (L) {
        lua_close(L);
    }
}

void *lua_engine::mem_alloc(void *ud, void *ptr, size_t osize, size_t nsize) {
    auto *allocator = reinterpret_cast<mm::allocator *>(ud);
    // printf("%lu %lu %p\n", osize, nsize, ptr);

    if (nsize == 0) {
        if (ptr) {
            allocator->free(ptr);
        }
        return nullptr;
    } else {
        auto *new_ptr = allocator->allocate(nsize, 8);
        if (new_ptr && ptr && osize) {
            memcpy(new_ptr, ptr, osize);
            allocator->free(ptr);
        }
        return new_ptr;
    }
}

void lua_engine::exec(const char *code) {
    luaL_dostring(L, code);
}

} // namespace niu2x
