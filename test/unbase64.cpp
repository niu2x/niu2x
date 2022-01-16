#include <sstream>

#include <niu2x/pipe.h>

int main()
{
    nx::pipe::filter::unbase64_t unbase64;
    nx::pipe::filter::cut_t cut('\n');

    nx::pipe::source_t(std::cin) | cut | unbase64 | nx::pipe::sink_t(std::cout);

    return 0;
}