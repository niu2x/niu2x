#include <sstream>

#include <niu2x/pipe.h>

int main(int argc, char* argv[])
{
    nx::pipe::filter::decrypt decrypt(
        argv[1], (const uint8_t*)argv[2], (const uint8_t*)argv[3]);

    nx::pipe::source(std::cin) | decrypt | nx::pipe::sink(std::cout);

    return 0;
}