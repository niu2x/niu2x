#include <sstream>

#include <niu2x/io.h>

int main()
{
    nx::io::filter::lower lower;
    nx::io::filter::hex hex;
    nx::io::filter::unhex unhex;
    nx::io::filter::zlib zlib;
    nx::io::filter::unzlib unzlib;
    // nx::io::source(std::cin) | base64 | nx::io::sink(std::cout);
    std::stringstream ss;
    nx::io::source(std::cin) | unzlib | nx::io::sink(std::cout);

    return 0;
}