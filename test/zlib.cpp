#include <sstream>

#include <niu2x/pipe.h>

int main()
{
    nx::pipe::filter::zlib_t zlib;
    nx::pipe::source_t(std::cin) | zlib | nx::pipe::sink_t(std::cout);
    return 0;
}