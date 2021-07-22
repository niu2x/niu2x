#include <iostream>

#define NX_LOG_LEVEL 5

#include <niu2x/lua_engine.h>
#include <niu2x/memory.h>
#include <niu2x/io.h>
#include <niu2x/misc/ringbuffer.h>

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

            while ((status = bs.get_elem(&c)) != io::status::eof) {
                if (status == io::status::ok)
                    std::cout << c;
            }
        }

        {
            io::byte_source bs(buffer.data(), buffer.size());
            bs | io::sink::cout;
        }

        {

            io::byte_source bs(buffer.data(), buffer.size());
            io::pipe(bs, io::filter::hex_encode, io::sink::cout);
        }

        {
            io::byte_source bs(buffer.data(), buffer.size());
            io::pipe(bs, io::sink::cerr);
        }

        {
        	io::sink::file f("./b.txt");
        	io::byte_source bs(buffer.data(), buffer.size());
            io::pipe(bs, f);
        }

        {
            std::cout << "\n: ";
            char message[] = "HHHHHHHH";
            io::byte_source bs((uint8_t*)(&message), 8);

            io::pipe(bs, io::filter::zlib_compress | io::filter::hex_encode,
                io::sink::cout);
            std::cout << "\n: ";
        }

    } catch (exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}