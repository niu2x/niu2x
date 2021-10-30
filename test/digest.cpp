#include <sstream>

#include <niu2x/io.h>

int main(int argc, char* argv[])
{
    nx::io::filter::hex hex;
    nx::io::filter::digest digest(argv[1]);
    nx::io::source(std::cin) | digest | hex | nx::io::sink(std::cout);
    std::cout << std::endl;
    return 0;
}