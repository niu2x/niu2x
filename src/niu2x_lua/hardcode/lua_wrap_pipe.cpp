#include "lua_wrap_pipe.h"

#define BYTES(a, b, c, d, e, f, g, h)                                          \
    0x##a, 0x##b, 0x##c, 0x##d, 0x##e, 0x##f, 0x##g, 0x##h,
namespace nxlua::hardcode {
unsigned long lua_wrap_pipe_length = 0x101;

unsigned char lua_wrap_pipe[] = { BYTES(1b, 4c, 4a, 2, a, b4, 1, 0) BYTES(
    2, c, 3, 7, 0, 1f, 36, 2) BYTES(0, 0, 39, 2, 1, 2, 12, 4) BYTES(1, 0, 27, 5,
    2, 0, 42, 2) BYTES(3, 2, 36, 3, 3, 0, 12, 5) BYTES(2, 0, 42, 3, 2, 4, 58,
    6) BYTES(13, 80, 36, 8, 4, 0, 39, 8) BYTES(5, 8, 2d, a, 0, 0, 12,
    b) BYTES(7, 0, 42, 8, 3, 2, f, 0) BYTES(8, 0, 58, 9, 6, 80, 2d,
    8) BYTES(1, 0, 12, a, 0, 0, 12, b) BYTES(7, 0, 42, 8, 3, 2, 12, 0) BYTES(8,
    0, 58, 8, 6, 80, 2d, 8) BYTES(2, 0, 12, a, 0, 0, 36, b) BYTES(6, 0, 38, b,
    7, b, 42, 8) BYTES(3, 2, 12, 0, 8, 0, 45, 6) BYTES(3, 3, 52, 6, eb, 7f, 4c,
    0) BYTES(2, 0, 2, c0, 1, c0, 0, c0) BYTES(b, 66, 69, 6c, 74, 65, 72,
    c) BYTES(69, 6e, 63, 6c, 75, 64, 65, a) BYTES(74, 61, 62, 6c, 65, b, 69,
    70) BYTES(61, 69, 72, 73, 6, 7c, a, 73) BYTES(70, 6c, 69, 74, b, 73, 74, 72)
        BYTES(69, 6e, 67, 44, 3, 0, 4, 0) BYTES(4, 0, 7, 36, 0, 0, 0, 36)
            BYTES(1, 1, 0, 35, 2, 2, 0, 33) BYTES(3, 3, 0, 37, 3, 0, 0, 32)
                BYTES(0, 0, 80, 4b, 0, 1, 0, 0) BYTES(1, 3, 0, 0, 8, 6d, 64, 35)
                    BYTES(b, 73, 68, 61, 32, 35, 36, 10) BYTES(70, 69, 70, 65,
                        5f, 64, 69, 67) BYTES(65, 73, 74, 9, 70, 69, 70, 65)
                        BYTES(0, 0, 0, 0, 0, 0, 0, 0) 0x0 };
} // namespace nxlua::hardcode
