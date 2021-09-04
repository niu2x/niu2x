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

rid create_tcp();
void destroy_tcp(rid tcp);

void tcp_connect(rid tcp, const char* ip, uint16_t port, nx_luafunction cb);
void tcp_listen(rid tcp, const char* ip, uint16_t port, nx_luafunction cb);

void tcp_read_start(rid tcp, nx_luafunction cb);
void tcp_read_stop(rid tcp);

void tcp_write(rid tcp, const void* buffer, size_t size);
void tcp_write(rid tcp, const void* buffer, size_t size, nx_luafunction);
bool tcp_alive(rid tcp_id);

} // namespace nxlua::aio

#endif