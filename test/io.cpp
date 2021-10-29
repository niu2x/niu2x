#include <sstream>

#include <niu2x/io.h>

int main()
{
    nx::io::filter::lower lower;
    nx::io::source(std::cin) | lower | nx::io::sink(std::cout);
    return 0;
}