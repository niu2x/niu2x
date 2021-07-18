#include <iostream>

#define NX_LOG_LEVEL 5

#include <niu2x/lua_engine.h>
#include <niu2x/memory.h>

using namespace nx;

int main()
{
    lua_engine l(32_k);
    printf("n\n");
    l.exec(R"STR(
    )STR");

    return 0;
}