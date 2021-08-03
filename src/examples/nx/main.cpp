// #include <common/cxxopts.h>

#include <niu2x/lua_engine.h>

/**
 * @brief      { function_description }
 *
 * @param[in]  argc  The count of arguments
 * @param      argv  The arguments array
 *
 * @return     { description_of_the_return_value }
 */
int main(int argc, char* argv[])
{

    if (argc < 2) {
        std::cerr << "too few arguments" << std::endl;
        exit(EXIT_FAILURE);
    }

    nx::lua_engine lua;
    lua.dostring("_G.sys={argv={}}");
    lua.set_global_variable("sys.argv", argv);
    lua.dofile(argv[1]);
    return 0;
}