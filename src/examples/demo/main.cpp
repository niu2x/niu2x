#include <iostream>

#define NX_LOG_LEVEL 5

#include <niu2x/lua_engine.h>
#include <niu2x/memory.h>
#include <niu2x/io.h>

using namespace nx;

int main()
{
    // lua_engine l(64_m);
    // l.dofile("main.lua");

	std::vector<uint8_t> buffer;
	try{
        io::read_file("./main.lua", buffer);

        {
            io::byte_source bs(buffer.data(), buffer.size());
            uint8_t c;
            io::status status;

            while ((status = bs.get_elem(c)) != io::status::eof) {
                if (status == io::status::ok)
                    std::cout << c;
            }
        }

        {
            io::byte_source bs(buffer.data(), buffer.size());
            uint8_t c;
            io::status status;

            uint8_t buf[16];
            size_t readn;

            while ((status = bs.get(16, buf, &readn)) != io::status::eof) {
                if (status == io::status::ok) {
                    fwrite(buf, 1, readn, stdout);
                }
            }
        }

    } catch (exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}