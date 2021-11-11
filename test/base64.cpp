#include <sstream>

#include <niu2x/pipe.h>

int main()
{
    nx::pipe::filter::lower lower;
    nx::pipe::filter::hex hex;
    nx::pipe::filter::unhex unhex;
    nx::pipe::filter::base64 base64;
    nx::pipe::filter::unbase64 unbase64;
    nx::pipe::filter::cut cut('\n');
    // nx::pipe::source(std::cin) | base64 | nx::pipe::sink(std::cout);

    std::stringstream ss;
    nx::pipe::source(std::cin) | base64 | nx::pipe::sink(std::cout);

    return 0;
}