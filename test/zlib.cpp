#include <sstream>

#include <niu2x/io.h>

int main()
{
    nx::io::filter::zlib zlib;
    nx::io::source(std::cin) | zlib | nx::io::sink(std::cout);
    return 0;
}