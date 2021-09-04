#include "aio.h"
#include "niu2x_lua/tolua/tolua++.h"

namespace nxlua::aio {

void run(int type) { nx::aio::run(type); }

rid create_idle(nx_luafunction callback)
{
    return nx::aio::create_idle([callback](auto id) {
        callback.call([id](struct lua_State* tolua_S) {
            void* tolua_obj = Mtolua_new((nx::aio::rid)(id));
            tolua_pushusertype(tolua_S, tolua_obj, "nx::aio::rid");
            tolua_register_gc(tolua_S, lua_gettop(tolua_S));
            return 1;
        });
    });
}
void destroy_idle(rid idle) { nx::aio::destroy_idle(idle); }

rid create_tcp() { return nx::aio::create_tcp(); }

void destroy_tcp(rid tcp) { nx::aio::destroy_tcp(tcp); }

void tcp_connect(rid tcp, const char* ip, uint16_t port, nx_luafunction cb)
{
    nx::aio::tcp_connect(tcp, ip, port, [cb](auto status, auto id) {
        cb.call([status, id](struct lua_State* tolua_S) {
            if (status == nx::ok) {
                lua_pushnil(tolua_S);
            } else {
                tolua_pushstring(tolua_S, "fail");
            }
            void* tolua_obj = Mtolua_new((nx::aio::rid)(id));
            tolua_pushusertype(tolua_S, tolua_obj, "nx::aio::rid");
            tolua_register_gc(tolua_S, lua_gettop(tolua_S));
            return 2;
        });
    });
}

void tcp_listen(rid tcp, const char* ip, uint16_t port, nx_luafunction cb)
{
    nx::aio::tcp_listen(tcp, ip, port, [cb](auto status, auto id) {
        cb.call([status, id](struct lua_State* tolua_S) {
            if (status == nx::ok) {
                lua_pushnil(tolua_S);
            } else {
                tolua_pushstring(tolua_S, "fail");
            }
            void* tolua_obj = Mtolua_new((nx::aio::rid)(id));
            tolua_pushusertype(tolua_S, tolua_obj, "nx::aio::rid");
            tolua_register_gc(tolua_S, lua_gettop(tolua_S));
            return 2;
        });
    });
}

void tcp_read_start(rid tcp, nx_luafunction cb)
{
    nx::aio::tcp_read_start(
        tcp, [cb](auto status, auto id, auto* ptr, auto size) {
            cb.call([status, id, ptr, size](struct lua_State* tolua_S) {
                if (status == nx::ok) {
                    lua_pushnil(tolua_S);
                } else {
                    tolua_pushstring(tolua_S, "fail");
                }

                void* tolua_obj = Mtolua_new((nx::aio::rid)(id));
                tolua_pushusertype(tolua_S, tolua_obj, "nx::aio::rid");
                tolua_register_gc(tolua_S, lua_gettop(tolua_S));

                lua_pushlstring(
                    tolua_S, reinterpret_cast<const char*>(ptr), size);

                return 3;
            });
        });
}
void tcp_read_stop(rid tcp) { nx::aio::tcp_read_stop(tcp); }

void tcp_write(rid tcp, const void* buffer, size_t size, nx_luafunction cb)
{
    nx::aio::tcp_write(tcp, buffer, size, [cb](auto status, auto id) {
        cb.call([status, id](struct lua_State* tolua_S) {
            if (status == nx::ok) {
                lua_pushnil(tolua_S);
            } else {
                tolua_pushstring(tolua_S, "fail");
            }

            void* tolua_obj = Mtolua_new((nx::aio::rid)(id));
            tolua_pushusertype(tolua_S, tolua_obj, "nx::aio::rid");
            tolua_register_gc(tolua_S, lua_gettop(tolua_S));

            return 2;
        });
    });
}

void tcp_write(rid tcp, const void* buffer, size_t size)
{
    nx::aio::tcp_write(tcp, buffer, size);
}

bool tcp_alive(rid tcp_id) { return nx::aio::tcp_alive(tcp_id); }

} // namespace nxlua::aio