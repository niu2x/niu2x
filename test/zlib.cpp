#include <sstream>

#include <niu2x/pipe.h>

int main()
{
    nx::pipe::filter::zlib zlib;
    nx::pipe::source(std::cin) | zlib | nx::pipe::sink(std::cout);
    return 0;
}