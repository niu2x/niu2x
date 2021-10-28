#include <niu2x/io.h>

int main()
{
    nx::io::source(std::cin) | nx::io::sink(std::cout);
    return 0;
}