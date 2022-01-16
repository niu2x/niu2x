#include <sstream>

#include <niu2x/pipe.h>

int main(int argc, char* argv[])
{
    nx::pipe::filter::encrypt_t encrypt(
        argv[1], (const uint8_t*)argv[2], (const uint8_t*)argv[3]);

    nx::pipe::source_t(std::cin) | encrypt | nx::pipe::sink_t(std::cout);

    return 0;
}