#ifndef NXLUA_TOLUA_FIX_H
#define NXLUA_TOLUA_FIX_H

#include <functional>

#include <niu2x/utils.h>

struct lua_State;
struct tolua_Error;

extern void tolua_pushnxcppstring(lua_State* L, const std::string& sz);
extern std::string tolua_tonxcppstring(lua_State* L, int idx, int def);
extern int tolua_isnxcppstring(
    lua_State* L, int idx, int def, tolua_Error* err);

class nx_luafunction {
public:
    nx_luafunction(lua_State* L, int idx);
    ~nx_luafunction();

    nx_luafunction(const nx_luafunction&) = default;
    nx_luafunction& operator=(const nx_luafunction&) = default;

    void call(const std::function<int(struct lua_State*)>& callback) const;

private:
    class luafunc_ref : private nx::noncopyable {
    public:
        luafunc_ref(lua_State* L, int idx);
        ~luafunc_ref();
        void call(const std::function<int(struct lua_State*)>& callback) const;

    private:
        lua_State* L_;
        uint32_t func_id_;

        static uint32_t next_func_id_;
    };

    std::shared_ptr<luafunc_ref> ref_;
};

extern nx_luafunction tolua_tonx_luafunction(lua_State* L, int idx, int def);
extern int tolua_isnx_luafunction(
    lua_State* L, int idx, int def, tolua_Error* err);

#endif