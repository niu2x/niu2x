#include "lua_init.h"

#define BYTES(a, b, c, d, e, f, g, h)                                          \
    0x##a, 0x##b, 0x##c, 0x##d, 0x##e, 0x##f, 0x##g, 0x##h,
namespace nx::hardcode {

unsigned long lua_init_length = 0x44;

unsigned char lua_init[] = { BYTES(1b, 4c, 4a, 2, a, 3d, 2, 0) BYTES(4, 0, 3, 0,
    8, 36, 0, 0) BYTES(0, 36, 1, 0, 0, 39, 1, 1) BYTES(1, 12, 3, 1, 0, 39, 1, 2)
        BYTES(1, 42, 1, 2, 2, 3d, 1, 1) BYTES(0, 4b, 0, 1, 0, b, 63, 72) BYTES(
            65, 61, 74, 65, f, 65, 76, 65) BYTES(6e, 74, 5f, 6c, 6f, 6f, 70, 8)
            BYTES(61, 69, 6f, 0, 0, 0, 0, 0) 0x0 };
} // namespace nx::hardcode
