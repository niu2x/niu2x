#include <iostream>

#include <niu2x/utils.h>

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "too few arguments" << std::endl;
        exit(EXIT_FAILURE);
    }

    nx::freelist_memory my_memory(16 * 1024 * 1024);
    nx::memory_proxy allocator(my_memory);

    for (int i = 0; i < 1024; i++) {
        auto* ptr = allocator.allocate(1024 * 256);
        printf("%d %p\n", i, ptr);
        allocator.free(ptr);
        if (!ptr)
            break;
    }

    // nx::lua_engine lua;
    // lua.dostring("_G.sys={argv={}}");
    // lua.set_global_variable("sys.argv", argv);
    // lua.dofile(argv[1]);
    return 0;
}