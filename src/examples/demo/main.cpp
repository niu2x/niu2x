#include <iostream>

#include <niu2x/lua_engine.h>
#include <niu2x/memory.h>

using namespace niu2x;

int main() {
    lua_engine l(1024 * 1024);

    l.exec("print('hello lua')");

    return 0;
}