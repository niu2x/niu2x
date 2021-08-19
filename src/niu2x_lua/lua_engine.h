#ifndef NXLUA_LUA_ENGINE_H
#define NXLUA_LUA_ENGINE_H

#include <vector>

#include <niu2x/utils.h>
#include <niu2x_lua/lua_utils.h>

struct lua_State;

namespace nxlua {

class lua_engine : private nx::noncopyable {
public:
    lua_engine(nx::memory_proxy);
    ~lua_engine();

    void dostring(const char*);
    void dobuffer(const nx::const_memref& mref)
    {
        lua_utils::dobuffer(L, mref);
    }
    void dofile(const char*);
    void compile(const char*, std::vector<uint8_t>& output);

    void set_global_variable(const char* name, const char* const sz_list[]);

private:
    lua_State* L;
    size_t memory_limit_;
    nx::memory_proxy allocator_;
    static void* mem_alloc(void* ud, void* ptr, size_t osize, size_t nsize);
};

} // namespace nxlua

#endif