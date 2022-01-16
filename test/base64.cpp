#include <sstream>

#include <niu2x/pipe.h>

int main()
{
    nx::pipe::filter::lower_t lower;
    nx::pipe::filter::hex_t hex;
    nx::pipe::filter::unhex_t unhex;
    nx::pipe::filter::base64_t base64;
    nx::pipe::filter::unbase64_t unbase64;
    nx::pipe::filter::cut_t cut('\n');
    // nx::pipe::source(std::cin) | base64 | nx::pipe::sink(std::cout);

    std::stringstream ss;
    nx::pipe::source_t(std::cin) | base64 | nx::pipe::sink_t(std::cout);

    return 0;
}