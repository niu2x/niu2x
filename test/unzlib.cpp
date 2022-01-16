#include <sstream>

#include <niu2x/pipe.h>

int main()
{
    nx::pipe::filter::unzlib_t unzlib;
    // nx::pipe::source(std::cin) | base64 | nx::pipe::sink(std::cout);
    nx::pipe::source_t(std::cin) | unzlib | nx::pipe::sink_t(std::cout);

    return 0;
}