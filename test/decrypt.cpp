#include <sstream>

#include <niu2x/pipe.h>

int main(int argc, char* argv[])
{
    nx::pipe::filter::decrypt_t decrypt(
        argv[1], (const uint8_t*)argv[2], (const uint8_t*)argv[3]);

    nx::pipe::source_t(std::cin) | decrypt | nx::pipe::sink_t(std::cout);

    return 0;
}