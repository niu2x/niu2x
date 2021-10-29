#include <sstream>

#include <niu2x/io.h>

int main()
{
    nx::io::filter::lower lower;
    nx::io::filter::hex hex;
    nx::io::filter::unhex unhex;
    nx::io::source(std::cin) | unhex | nx::io::sink(std::cout);
    return 0;
}