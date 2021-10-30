#include <sstream>

#include <niu2x/io.h>

int main(int argc, char* argv[])
{
    nx::io::filter::encrypt encrypt(
        argv[1], (const uint8_t*)argv[2], (const uint8_t*)argv[3]);

    nx::io::source(std::cin) | encrypt | nx::io::sink(std::cout);

    return 0;
}