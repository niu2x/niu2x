#ifndef NXLUA_BINDINGS_H
#define NXLUA_BINDINGS_H

struct lua_State;

namespace nxlua::bindings {

void openlib_log(lua_State* L);
}

#endif