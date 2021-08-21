#include <iostream>

#define NX_LOG_LEVEL 5

#include <niu2x/memory.h>
#include <niu2x/io.h>

using namespace nx;

int main()
{

    const uint8_t key[64] = { 0 };
    const uint8_t iv[64] = { 0 };

    nx::io::digest sha256("sha256");
    nx::io::cipher des(nx::io::cipher::encrypt, "des", key, iv);

    nx::io::pipe(nx::io::cin, nx::io::filter_proxy(&des), nx::io::cout);
    return 0;
}