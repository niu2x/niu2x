#include <sstream>

#include <niu2x/pipe.h>

int main(int argc, char* argv[])
{
    nx::pipe::filter::hex hex;
    nx::pipe::filter::digest digest(argv[1]);
    nx::pipe::source(std::cin) | digest | hex | nx::pipe::sink(std::cout);
    std::cout << std::endl;
    return 0;
}