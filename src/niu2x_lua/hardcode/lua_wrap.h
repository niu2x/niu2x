#ifndef lua_wrap__H
#define lua_wrap__H
#include <niu2x/utils.h>

#ifdef __cplusplus
extern "C" {
#endif

namespace nxlua::hardcode {
extern unsigned char lua_wrap[];
extern unsigned long lua_wrap_length;
const nx::const_memref lua_wrap_mref = { lua_wrap, lua_wrap_length };
} // namespace nxlua::hardcode

#ifdef __cplusplus
}
#endif

#endif
