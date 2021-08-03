#ifndef lua_init__H
#define lua_init__H
#include <niu2x/utils.h>

#ifdef __cplusplus
extern "C" {
#endif

namespace nx::hardcode {
extern unsigned char lua_init[];
extern unsigned long lua_init_length;
const memref lua_init_mref = { lua_init, lua_init_length };
} // namespace nx::hardcode

#ifdef __cplusplus
}
#endif

#endif
