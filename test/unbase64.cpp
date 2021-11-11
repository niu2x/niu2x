#include <sstream>

#include <niu2x/pipe.h>

int main()
{
    nx::pipe::filter::unbase64 unbase64;
    nx::pipe::filter::cut cut('\n');

    nx::pipe::source(std::cin) | cut | unbase64 | nx::pipe::sink(std::cout);

    return 0;
}