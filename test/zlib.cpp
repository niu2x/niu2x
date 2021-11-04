#include <sstream>

#include <niu2x/io.h>

namespace io = nx::io;
int main()
{
    io::filter::zlib zlib;
    io::source(std::cin) | zlib | io::sink(std::cout);
    return 0;
}