#include <iostream>

#include <niu2x/utils.h>
#include <niu2x_lua/lua_engine.h>

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "too few arguments" << std::endl;
        exit(EXIT_FAILURE);
    }

    nx::freelist_memory my_memory(16 * 1024 * 1024);
    nx::memory_proxy allocator(my_memory);

    nxlua::lua_engine lua(allocator);
    lua.dostring("_G.sys={argv={}}");
    lua.set_global_variable("sys.argv", argv);
    lua.dofile(argv[1]);
    return 0;
}