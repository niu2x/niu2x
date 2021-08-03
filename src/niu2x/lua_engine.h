#ifndef NX_LUA_ENGINE_H
#define NX_LUA_ENGINE_H

#include <boost/noncopyable.hpp>

#include <niu2x/memory.h>
#include <niu2x/utils.h>

struct lua_State;

namespace nx {

struct lua_utils {
    static void dobuffer(lua_State* L, const memref& mref);
};

class API lua_engine : private boost::noncopyable {
public:
    lua_engine(size_t memory_limit = 16_m);
    ~lua_engine();

    void dostring(const char*);
    void dobuffer(const memref& mref) { lua_utils::dobuffer(L, mref); }
    void dofile(const char*);
    void compile(const char*, std::vector<uint8_t>& output);

    void set_global_variable(const char* name, const char* const sz_list[]);

private:
    lua_State* L;
    size_t memory_limit_;
    mm::freelist_memory_t memory_;
    mm::allocator allocator_;

    static void* mem_alloc(void* ud, void* ptr, size_t osize, size_t nsize);
};

} // namespace nx

#endif