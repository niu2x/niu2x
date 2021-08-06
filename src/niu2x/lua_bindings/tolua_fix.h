#ifndef NX_LB_TOLUA_FIX_H
#define NX_LB_TOLUA_FIX_H

#include <niu2x/utils.h>
#include <niu2x/aio.h>

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

    template <class... Params>
    void operator()(Params&&... params)
    {
        ref_->operator()(std::forward<Params>(params)...);
    }

private:
    class luafunc_ref : private nx::noncopyable {
    public:
        luafunc_ref(lua_State* L, int idx);
        ~luafunc_ref();

        void operator()(nx::aio::status status, nx::misc::rid rid);
        void operator()(nx::aio::status status, nx::misc::rid rid,
            const uint8_t* data, size_t len);

    private:
        lua_State* L_;
        uint32_t func_id_;

        static uint32_t next_func_id_;
    };

    std::shared_ptr<luafunc_ref> ref_;
};

// extern void tolua_pushnx_luafunction(lua_State* L, const std::string& sz);
extern nx_luafunction tolua_tonx_luafunction(lua_State* L, int idx, int def);
extern int tolua_isnx_luafunction(
    lua_State* L, int idx, int def, tolua_Error* err);

#endif