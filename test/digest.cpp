#include <sstream>

#include <niu2x/pipe.h>

int main(int argc, char* argv[])
{
    nx::pipe::filter::hex_t hex;
    nx::pipe::filter::digest_t digest(argv[1]);
    nx::pipe::source_t(std::cin) | digest | hex | nx::pipe::sink_t(std::cout);
    std::cout << std::endl;
    return 0;
}