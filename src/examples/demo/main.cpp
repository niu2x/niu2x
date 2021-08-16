#include <iostream>

#define NX_LOG_LEVEL 5

#include <niu2x/memory.h>
#include <niu2x/io.h>

using namespace nx;

int main()
{
    nx::io::pipe(nx::io::cin, nx::io::hex, nx::io::unhex, nx::io::cout);
    return 0;
}