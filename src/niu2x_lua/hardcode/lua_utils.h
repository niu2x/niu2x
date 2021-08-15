#ifndef lua_utils__H
#define lua_utils__H
#include <niu2x/utils.h>

#ifdef __cplusplus
extern "C" {
#endif

namespace nxlua::hardcode {
extern unsigned char lua_utils[];
extern unsigned long lua_utils_length;
const nx::const_memref lua_utils_mref = { lua_utils, lua_utils_length };
} // namespace nxlua::hardcode

#ifdef __cplusplus
}
#endif

#endif
