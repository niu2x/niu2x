#include <iostream>

#define NX_LOG_LEVEL 5

#include <niu2x/memory.h>
#include <niu2x/io.h>
#include <niu2x/aio.h>

using namespace nx;

int main()
{

    nx::aio::create_idle([](auto rid) {
        printf("hello world!\n");
        nx::aio::destroy_idle(rid);
    });
    nx::aio::run();
    return 0;
}