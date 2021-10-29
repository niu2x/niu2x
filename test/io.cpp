#include <sstream>

#include <niu2x/io.h>

int main()
{
    nx::io::filter::lower lower;
    nx::io::filter::hex hex;
    nx::io::filter::unhex unhex;
    nx::io::filter::base64 base64;
    nx::io::filter::unbase64 unbase64;
    nx::io::filter::cut cut('\n');
    // nx::io::source(std::cin) | base64 | nx::io::sink(std::cout);

    std::stringstream ss;
    nx::io::source(std::cin) | cut | unbase64 | nx::io::sink(std::cout);

    return 0;
}