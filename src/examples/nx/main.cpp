// #include <common/cxxopts.h>

#include <niu2x/lua_engine.h>

int main(int argc, char* argv[])
{

    if (argc < 2) {
        std::cerr << "too few arguments" << std::endl;
        exit(EXIT_FAILURE);
    }

    nx::lua_engine lua;
    lua.dofile(argv[1]);
    return 0;
}