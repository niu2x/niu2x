#ifndef NXLUA_AIO_H
#define NXLUA_AIO_H

#include <niu2x/aio.h>
#include <niu2x_lua/tolua_fix.h>

namespace nxlua::aio {

enum type {
    wait_all,
    wait_once,
    no_wait,
};

void run(int type = wait_all);

using rid = nx::aio::rid;

rid create_idle(nx_luafunction);
void destroy_idle(rid idle);

} // namespace nxlua::aio

#endif