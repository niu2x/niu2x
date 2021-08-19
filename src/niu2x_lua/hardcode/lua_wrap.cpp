#include "lua_wrap.h"

#define BYTES(a, b, c, d, e, f, g, h)                                          \
    0x##a, 0x##b, 0x##c, 0x##d, 0x##e, 0x##f, 0x##g, 0x##h,
namespace nxlua::hardcode {
unsigned long lua_wrap_length = 0x97;

unsigned char lua_wrap[] = { BYTES(1b, 4c, 4a, 2, a, 6e, 0, 2) BYTES(c, 1, 5, 0,
    12, 36, 2, 0) BYTES(0, 39, 2, 1, 2, 12, 4, 1) BYTES(0, 27, 5, 2, 0, 42, 2,
    3) BYTES(2, 36, 3, 3, 0, 12, 5, 2) BYTES(0, 42, 3, 2, 4, 58, 6, 6) BYTES(80,
    2d, 8, 0, 0, 12, a, 0) BYTES(0, 36, b, 4, 0, 38, b, 7) BYTES(b, 42, 8, 3, 2,
    12, 0, 8) BYTES(0, 45, 6, 3, 3, 52, 6, f8) BYTES(7f, 4c, 0, 2, 0, 0, c0, b)
        BYTES(66, 69, 6c, 74, 65, 72, b, 69) BYTES(70, 61, 69, 72, 73, 6, 7c, a)
            BYTES(73, 70, 6c, 69, 74, b, 73, 74) BYTES(
                72, 69, 6e, 67, 21, 3, 0, 2) BYTES(0, 2, 0, 5, 36, 0, 0, 0)
                BYTES(33, 1, 1, 0, 37, 1, 0, 0) BYTES(32, 0, 0, 80, 4b, 0, 1, 0)
                    BYTES(0, 9, 70, 69, 70, 65, 0, 0) 0x0 };
}
