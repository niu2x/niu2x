#include "openlib.h"

namespace nx::lua_bindings {

static char source[limits::max_path] = { 0 };
static int lineno = 0;

static void fetch_backtrace(lua_State* L)
{
    // unused(L);
    lua_getglobal(L, "debug"); // debug
    lua_getfield(L, -1, "getinfo"); // debug getinfo
    lua_pushnumber(L, 2); // debug getinfo 0
    lua_call(L, 1, 1); // debug result
    lua_getfield(L, -1, "source"); // debug result source
    strcpy(source, lua_tostring(L, -1));
    lua_pop(L, 1); // debug result

    lua_getfield(L, -1, "currentline"); // debug result currentline
    lineno = lua_tonumber(L, -1);
    lua_pop(L, 3); // debug result
}

#define DEF_LOG(level_tag, p_level)                                            \
    static int log_##level_tag(lua_State* L)                                   \
    {                                                                          \
        if (lua_gettop(L)) {                                                   \
            fetch_backtrace(L);                                                \
            log::write(                                                        \
                log::level::p_level, source, lineno, lua_tostring(L, -1));     \
        }                                                                      \
        return 0;                                                              \
    }

DEF_LOG(t, trace)
DEF_LOG(d, debug)
DEF_LOG(w, warning)
DEF_LOG(e, error)
DEF_LOG(f, fatal)

static const struct luaL_Reg lib[] = {
    { "t", log_t },
    { "d", log_d },
    { "w", log_w },
    { "e", log_e },
    { "f", log_f },
    { nullptr, nullptr },
};

void openlib_log(lua_State* L) { luaL_register(L, "log", lib); }

} // namespace nx::lua_bindings