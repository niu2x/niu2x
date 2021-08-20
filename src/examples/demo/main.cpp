#include <iostream>

#define NX_LOG_LEVEL 5

#include <niu2x/memory.h>
#include <niu2x/io.h>

using namespace nx;

int main()
{
    nx::io::digest sha256("sha256");
    nx::io::pipe(
        nx::io::cin, nx::io::filter_proxy(&sha256), nx::io::hex, nx::io::cout);
    return 0;
}