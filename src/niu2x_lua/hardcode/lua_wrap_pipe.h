#ifndef lua_wrap_pipe__H
#define lua_wrap_pipe__H
#include <niu2x/utils.h>

#ifdef __cplusplus
extern "C" {
#endif

namespace nxlua::hardcode {
extern unsigned char lua_wrap_pipe[];
extern unsigned long lua_wrap_pipe_length;
const nx::const_memref lua_wrap_pipe_mref
    = { lua_wrap_pipe, lua_wrap_pipe_length };
} // namespace nxlua::hardcode

#ifdef __cplusplus
}
#endif

#endif
