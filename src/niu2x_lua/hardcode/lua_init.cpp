#include "lua_init.h"

#define BYTES(a, b, c, d, e, f, g, h)                                          \
    0x##a, 0x##b, 0x##c, 0x##d, 0x##e, 0x##f, 0x##g, 0x##h,
namespace nxlua::hardcode {
	
unsigned long lua_init_length = 0x12;
unsigned char lua_init[] = { BYTES(1b, 4c, 4a, 2, a, b, 2, 0)
        BYTES(1, 0, 0, 0, 1, 4b, 0, 1) BYTES(0, 0, 0, 0, 0, 0, 0, 0) 0x0 };
}
