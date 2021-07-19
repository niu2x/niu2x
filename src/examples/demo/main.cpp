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
		io::read_file("./x.lua", buffer);
		std::cout << buffer.data() << std::endl;
	}
	catch(exception &e){
		std::cerr << e.what() << std::endl;
	}

    return 0;
}