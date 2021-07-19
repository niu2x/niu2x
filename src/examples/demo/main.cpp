#include <iostream>

#define NX_LOG_LEVEL 5

#include <niu2x/lua_engine.h>
#include <niu2x/memory.h>

using namespace nx;

int main()
{
    lua_engine l(64_m);
    l.dofile("main.lua");
    return 0;
}