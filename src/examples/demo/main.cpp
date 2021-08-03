#include <iostream>

#define NX_LOG_LEVEL 5

#include <niu2x/lua_engine.h>
#include <niu2x/memory.h>
#include <niu2x/io.h>
#include <niu2x/os.h>
#include <niu2x/async_io.h>
#include <niu2x/global.h>
#include <niu2x/misc/ringbuffer.h>
#include <niu2x/lua_bindings/cvt.h>

using namespace nx;

int main()
{
    // lua_engine l(64_m);
    // l.dofile("main.lua");

	std::vector<uint8_t> buffer;
	try{
        io::read_file("./main.lua", buffer);

        {
            io::source::bytes bs(buffer.data(), buffer.size());
            uint8_t c;
            io::status status;

            while ((status = bs.get_elem(&c)) != io::status::eof) {
                if (status == io::status::ok)
                    std::cout << c;
            }
        }

        {
            io::source::bytes bs(buffer.data(), buffer.size());
            bs | io::sink::cout;
        }

        {

            io::source::bytes bs(buffer.data(), buffer.size());
            io::pipe(bs, io::filter::hex_encode, io::sink::cout);
        }

        {
            io::source::bytes bs(buffer.data(), buffer.size());
            io::pipe(bs, io::sink::cerr);
        }

        {
        	io::sink::file f("./b.txt");
        	io::source::bytes bs(buffer.data(), buffer.size());
            io::pipe(bs, f);
        }

        {
            std::cout << "\n: ";
            char message[] = "HHHHHHHH";
            io::source::bytes bs((uint8_t*)(&message), 8);

            io::pipe(bs, (io::filter::zlib_compress | io::filter::hex_encode),
                io::sink::cout);
            std::cout << "\n: ";
        }

        {
            std::cout << nx::lua_bindings::cvt_utils::hex_encode("hello ")
                      << std::endl;
        }

        {
            int counter = 0;
            auto my_loop = nx::aio::event_loop::create();
            my_loop->create_idle([&my_loop, &counter](auto idle) {
                printf("hello world\n");
                if (counter++ >= 8)
                    my_loop->destroy_idle(idle);
            });

            auto tcp = my_loop->create_tcp();
            my_loop->connect(tcp, "8.8.8.8", 53, [](auto status, auto conn) {
                printf("what happen %d\n", status);
            });

            my_loop->run(nx::aio::event_loop::type::wait);
        }

        {
            std::cout << "uv_event_loop size: " << sizeof(nx::aio::event_loop)
                      << std::endl;
        }

        {
            std::cout << "pid: " << nx::os::pid() << std::endl;
            std::cout << "ppid: " << nx::os::ppid() << std::endl;
        }

        // {
        //     execl("/bin/vi", "vi", nullptr);
        // }

    } catch (exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}