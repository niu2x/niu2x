#ifndef lua_utils__H
#define lua_utils__H
#include <niu2x/utils.h>

#ifdef __cplusplus
extern "C" {
#endif

namespace nx::hardcode {
extern unsigned char lua_utils[];
extern unsigned long lua_utils_length;
const memref lua_utils_mref={lua_utils, lua_utils_length};
}

#ifdef __cplusplus
}
#endif



#endif
