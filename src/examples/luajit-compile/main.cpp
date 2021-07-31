#include <niu2x/lua_engine.h>
#include <niu2x/io.h>

int main(int argc, char* argv[])
{

    nx::lua_engine lua;

    std::vector<uint8_t> buffer;

    {
        nx::io::sink::adapter<uint8_t, std::vector<uint8_t>> sink(buffer);
        nx::io::pipe(nx::io::source::cin, sink);
    }

    {
        std::vector<uint8_t> bytecode;
        lua.compile((const char*)buffer.data(), bytecode);
        buffer = std::move(bytecode);
    }

    {
        nx::io::source::bytes source(buffer.data(), buffer.size());
        nx::io::pipe(source, nx::io::sink::cout);
    }

    return 0;
}