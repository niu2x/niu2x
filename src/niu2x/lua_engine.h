#ifndef NX_LUA_ENGINE_H
#define NX_LUA_ENGINE_H

#include <boost/noncopyable.hpp>

#include <niu2x/memory.h>
#include <niu2x/utils.h>

class lua_State;

namespace nx {

class API lua_engine : private boost::noncopyable {
public:
    lua_engine(size_t memory_limit = 16 * 1024 * 1024);
    ~lua_engine();

    void dostring(const char*);
    void dofile(const char*);

private:
    lua_State* L;
    size_t memory_limit_;
    mm::freelist_memory_t memory_;
    mm::allocator allocator_;

    static void* mem_alloc(void* ud, void* ptr, size_t osize, size_t nsize);
};
} // namespace nx

#endif