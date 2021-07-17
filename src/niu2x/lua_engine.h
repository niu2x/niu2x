#ifndef NIU2X_LUA_ENGINE_H
#define NIU2X_LUA_ENGINE_H

#include <boost/noncopyable.hpp>

#include <niu2x/utils.h>
#include <niu2x/memory.h>

class lua_State;

namespace niu2x {

class API lua_engine : private boost::noncopyable{
public:
    lua_engine(size_t memory_limit = 16 * 1024 * 1024);
    ~lua_engine();

    void exec(const char *);

private:
    lua_State *           L;
    size_t                memory_limit_;
    mm::freelist_memory_t memory_;
    mm::allocator         allocator_;

    static void *mem_alloc(void *ud, void *ptr, size_t osize, size_t nsize);
};

}

#endif