#ifndef lua_export_luabindings__H
#define lua_export_luabindings__H
#include <niu2x/utils.h>

#ifdef __cplusplus
extern "C" {
#endif

namespace nx::hardcode {
extern unsigned char lua_export_luabindings[];
extern unsigned long lua_export_luabindings_length;
const memref lua_export_luabindings_mref={lua_export_luabindings, lua_export_luabindings_length};
}

#ifdef __cplusplus
}
#endif



#endif
