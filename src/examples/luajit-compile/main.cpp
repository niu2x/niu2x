#include <niu2x_lua/lua_engine.h>
#include <niu2x/io.h>

int main(int argc, char* argv[])
{
    nx::mallocfree_memory memory;
    nx::memory_proxy allocator(&memory);

    nxlua::lua_engine lua(allocator);

    std::vector<uint8_t> buffer;

    {
        nx::io::vector sink(buffer);
        nx::io::pipe(nx::io::cin, nx::io::sink_proxy(&sink));
        buffer.push_back(0);
    }

    {
        std::vector<uint8_t> bytecode;
        lua.compile((const char*)buffer.data(), bytecode);
        buffer = std::move(bytecode);
    }

    {
        nx::io::inbuf source(buffer.data(), buffer.size());
        nx::io::pipe(nx::io::source_proxy(&source), nx::io::cout);
    }

    return 0;
}