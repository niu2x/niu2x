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

    } catch (exception& e) {
        std::cerr << e.what() << std::endl;
    }

    misc::ringbuffer<int, 5> ring;

    for(int i = 0; i < 16; i +=4){
    	int array[] = {i, i + 1, i + 2};
	    ring.put(array, NX_ARRAY_SIZE(array), nullptr);
    }

	{
		int c;
		misc::rw_status status;
		while((status = ring.get_elem(&c)) != misc::rw_status::eof){
			if(status == misc::rw_status::ok){
				std::cout << c << " "<< std::endl; 
			}
		}
	}    



	for(int i = 0; i < 16; i +=4){
    	int array[] = {i, i + 1};
	    ring.put(array, NX_ARRAY_SIZE(array), nullptr);
    }

	{
		int c;
		misc::rw_status status;
		while((status = ring.get_elem(&c)) != misc::rw_status::eof){
			if(status == misc::rw_status::ok){
				std::cout << c << " "<< std::endl; 
			}
		}
    }

    {
        misc::ringbuffer<char, 8> rb;
        rb.put_elem('A');
        rb.put_elem('A');
        rb.put_elem('A');
        rb.put_elem('d');

        rb.put_elem('a');
        rb.put_elem('b');
        rb.put_elem('c');
        rb.put_elem('d');
        rb.put_elem('1');
        rb.put_elem('2');
        rb.put_elem('3');
        rb.put_elem('4');

        auto ar = rb.continuous_elems();
        std::cout.write(ar.base, ar.size);
        rb.pop(4);
        ar = rb.continuous_elems();
        std::cout.write(ar.base, ar.size);
    }

    return 0;
}