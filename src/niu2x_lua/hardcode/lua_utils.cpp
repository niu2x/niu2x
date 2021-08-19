#include "lua_utils.h"

#define BYTES(a, b, c, d, e, f, g, h)                                          \
    0x##a, 0x##b, 0x##c, 0x##d, 0x##e, 0x##f, 0x##g, 0x##h,
namespace nxlua::hardcode {
unsigned long lua_utils_length = 0x3c8;

unsigned char lua_utils[] = { BYTES(1b, 4c, 4a, 2, a, 99, 5, 0) BYTES(5, 18, 0,
    17, 1, 87, 1, e) BYTES(0, 2, 0, 58, 5, 1, 80, 29) BYTES(2, e8, 3, e, 0, 3,
    0, 58) BYTES(5, 1, 80, 29, 3, 7, 0, e) BYTES(0, 4, 0, 58, 5, 1, 80, 35)
        BYTES(4, 0, 0, 29, 5, 0, 0, 34) BYTES(6, 0, 0, 36, 7, 1, 0, 12) BYTES(
            9, 0, 0, 42, 7, 2, 2, 7) BYTES(7, 2, 0, 58, 7, 5f, 80, 36) BYTES(7,
            3, 0, 12, 9, 4, 0, 42) BYTES(7, 2, 4, 48, a, 7, 80, 5) BYTES(0, b,
            0, 58, c, 5, 80, 27) BYTES(c, 4, 0, 12, d, a, 0, 27) BYTES(e, 5, 0,
            26, c, e, c, 4c) BYTES(c, 2, 0, 46, a, 3, 3, 52) BYTES(a, f7, 7f,
            39, 7, 6, 4, 3c) BYTES(0, 7, 4, 36, 7, 2, 0, 39) BYTES(7, 7, 7, 12,
            9, 6, 0, 27) BYTES(a, 8, 0, 42, 7, 3, 1, 39) BYTES(7, 6, 4, 36,
            8, 3, 0, 12) BYTES(a, 0, 0, 42, 8, 2, 4, 48) BYTES(b, 35, 80, c, d,
            1, 0, 58) BYTES(d, 1, 80, 27, d, 9, 0, c) BYTES(e, 1,
            0, 58, e, 1, 80, 27) BYTES(e, 9, 0, 26, d, e, d, 36) BYTES(e, 2, 0,
            39, e, 7, e, 12) BYTES(10, 6, 0, 12, 11, d, 0, 36) BYTES(12, a,
            0, 12, 14, b, 0, 42) BYTES(12, 2, 2, 27, 13, b, 0, 26) BYTES(11, 13,
            11, 42, e, 3, 1, 12) BYTES(e, 7, 0, 27, f, c, 0, 36) BYTES(10, a,
            0, 12, 12, b, 0, 42) BYTES(10, 2, 2, 26, e, 10, e, 3d) BYTES(e, 6,
            4, 36, e, 2, 0, 39) BYTES(e, 7, e, 12, 10, 6, 0, 5) BYTES(c, 0, 0,
            58, 11, 2, 80, 27) BYTES(11, d, 0, 58, 12, 8, 80, 36) BYTES(11, 2,
            0, 39, 11, a, 11, 12) BYTES(13, c,
            0, 12, 14, d, 0, 12) BYTES(15, 2, 0, 12, 16, 3, 0, 12) BYTES(17, 4,
            0, 42, 11, 6, 2, 42) BYTES(e, 3, 1, 16, 5, 0, 5, 1) BYTES(2, 5,
            0, 58, e, a, 80, 36) BYTES(e, 2, 0, 39, e, 7, e, 12) BYTES(10, 6, 0,
            c, 11, d, 0, 58) BYTES(11, 1, 80, 27, 11, e, 0, 27) BYTES(12, f,
            0, 26, 11, 12, 11, 42) BYTES(e, 3, 1, 58, 8, 2, 80, 46) BYTES(b, 3,
            3, 52, b, c9, 7f, 36) BYTES(8, 2, 0, 39, 8, 7, 8, 12)
            BYTES(a, 6, 0, f, 0, 1, 0, 58) BYTES(b, 7, 80, c, b, 1, 0, 58)
                BYTES(b, 1, 80, 27, b, e, 0, 27) BYTES(c, 10, 0, 26, b, c, b, e)
                    BYTES(0, b, 0, 58, c, 1, 80, 27) BYTES(
                        b, 11, 0, 42, 8, 3, 1, 58) BYTES(7, 14, 80, 36, 7, 1, 0,
                        12) BYTES(9, 0, 0, 42, 7, 2, 2, 7) BYTES(7, 12, 0, 58,
                        7, 6, 80, 36) BYTES(7, 12, 0, 39, 7, 13, 7,
                        27) BYTES(9, 14, 0, 12, a, 0, 0, 42) BYTES(7, 3, 2, 12,
                        0, 7, 0, 36) BYTES(7, 2, 0, 39, 7, 7, 7, 12) BYTES(9, 6,
                        0, 36, a, a, 0, 12) BYTES(c, 0, 0, 42, a, 2, 2, 27)
                        BYTES(b, 15, 0, 26, a, b, a, 42) BYTES(7, 3, 1, 36, 7,
                            2, 0, 39) BYTES(7, 16, 7, 12, 9, 6, 0, 44) BYTES(7,
                            2, 0, b, 63, 6f, 6e, 63) BYTES(61, 74, 7, 2c, a,
                            7, 25, 71) BYTES(b, 66, 6f, 72, 6d, 61, 74,
                            b) BYTES(73, 74, 72, 69, 6e, 67, 6, 7d) BYTES(8, 7d,
                            2c, a, 9, 2e, 2e, 2e) BYTES(a, 5, e, 3c, 70, 61, 72,
                            65) BYTES(6e, 74, 3e, 2c, 6, 2e, 8, 20) BYTES(3d,
                            20, d, 74, 6f, 73, 74, 72) BYTES(69, 6e, 67, 6, 20,
                            7, 7b, a) BYTES(b, 69, 6e, 73, 65, 72, 74,
                            9) BYTES(6e, 61, 6d, 65, 8, 3e, 2c, a) BYTES(6, 3c,
                            a, 70, 61, 69, 72, 73) BYTES(a, 74, 61, 62, 6c, 65,
                            9, 74) BYTES(79, 70, 65, 1, 0, 1, 9, 6e) BYTES(61,
                            6d, 65, 8, 74, 6f, 70, 2) BYTES(e3, 1, 0, 3, e, 0,
                            6, 1) BYTES(2f, 36, 3, 0, 0, 39, 3, 1)
                            BYTES(3, 12, 5, 0, 0, 12, 6, 1) BYTES(0, 42, 3, 3,
                                2, b, 3, 0) BYTES(0, 58, 3, 3, 80, 34, 3, 3)
                                BYTES(0, 3e, 0, 1, 3, 4c, 3, 2) BYTES(0, a, 2,
                                    0, 0, 58, 3, 3) BYTES(80, 29, 3, 1, 0, 1, 2,
                                    3) BYTES(0, 58, 3, 1, 80, 29, 2, 0) BYTES(0,
                                    34, 3, 0, 0, 27, 4, 2) BYTES(0, 12, 5, 1, 0,
                                    27, 6, 3) BYTES(0, 26, 4, 6, 4, 29, 5, 0)
                                    BYTES(0, 2b, 6, 0, 0, 36, 7, 0) BYTES(0, 39,
                                        7, 4, 7, 12, 9, 0) BYTES(0, 12, a, 4, 0,
                                        42, 7, 3) BYTES(4, 58, a, 6, 80, 16, 5,
                                        0) BYTES(5, 3c, a, 5, 3, 12, 6, b)
                                        BYTES(0, 5, 5, 2, 0, 58, c, 1) BYTES(
                                            80, 58, 7, 2, 80, 45, a, 3) BYTES(3,
                                            52, a, f8, 7f, 4, 5, 2) BYTES(0, 58,
                                            7, 7, 80, 16, 7, 0) BYTES(5, 36, 8,
                                            0, 0, 39, 8, 5) BYTES(8, 12, a, 0,
                                            0, 12, b, 6) BYTES(0, 42, 8, 3, 2,
                                            3c, 8, 7) BYTES(3, 4c, 3, 2, 0, 8,
                                            73, 75) BYTES(62, b, 67, 6d, 61, 74,
                                            63, 68) BYTES(7, 28, 29, 9, 28, 2e,
                                            2d, 29) BYTES(9, 66, 69, 6e, 64, b,
                                            73, 74) BYTES(72, 69, 6e, 67, 2, 41,
                                            3, 0) BYTES(2, 0, 6, 0, 7, 36, 0,
                                            0) BYTES(0, 33, 1, 2, 0, 3d, 1,
                                            1) BYTES(0, 36, 0, 3, 0, 33, 1,
                                            5) BYTES(0, 3d, 1, 4, 0, 4b, 0, 1)
                                            BYTES(0, 0, a, 73, 70, 6c, 69, 74)
                                                BYTES(b, 73, 74, 72, 69, 6e, 67,
                                                    0) BYTES(d, 74, 6f, 73, 74,
                                                    72, 69, 6e) BYTES(67, a, 74,
                                                    61, 62, 6c, 65, 0) 0x0 };
}
