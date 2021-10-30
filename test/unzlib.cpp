#include <sstream>

#include <niu2x/io.h>

int main()
{
    nx::io::filter::unzlib unzlib;
    // nx::io::source(std::cin) | base64 | nx::io::sink(std::cout);
    nx::io::source(std::cin) | unzlib | nx::io::sink(std::cout);

    return 0;
}