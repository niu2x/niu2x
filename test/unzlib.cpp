#include <sstream>

#include <niu2x/pipe.h>

int main()
{
    nx::pipe::filter::unzlib unzlib;
    // nx::pipe::source(std::cin) | base64 | nx::pipe::sink(std::cout);
    nx::pipe::source(std::cin) | unzlib | nx::pipe::sink(std::cout);

    return 0;
}