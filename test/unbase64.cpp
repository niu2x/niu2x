#include <sstream>

#include <niu2x/io.h>

namespace io = nx::io;

int main()
{
    nx::io::filter::unbase64 unbase64;
    nx::io::filter::cut cut('\n');

    nx::io::source(std::cin) | cut | unbase64 | nx::io::sink(std::cout);

    return 0;
}