#include "lua_export_luabindings.h"


    #define BYTES(a,b,c,d,e,f,g,h) 0x##a,0x##b,0x##c,0x##d,0x##e,0x##f,0x##g,0x##h,
namespace nx::hardcode {

unsigned long lua_export_luabindings_length = 0x83;

unsigned char lua_export_luabindings[] = { BYTES(1b, 4c, 4a, 2, a, 7c, 2,
    0) BYTES(7, 0, 4, 0, 17, 36, 0, 0) BYTES(0, 36, 2, 1, 0, 39, 2, 2) BYTES(2,
    42, 0, 2, 4, 48, 3, 2) BYTES(80, 36, 5, 1, 0, 3c, 4, 3) BYTES(5, 46, 3, 3,
    3, 52, 3, fc) BYTES(7f, 36, 0, 1, 0, 2b, 1, 0) BYTES(0, 3d, 1, 2, 0, 36, 0,
    0) BYTES(0, 36, 2, 1, 0, 42, 0, 2) BYTES(4, 48, 3, 2, 80, 36, 5, 3)
        BYTES(0, 3c, 4, 3, 5, 46, 3, 3) BYTES(3, 52, 3, fc, 7f, 2b, 0, 0)
            BYTES(0, 37, 0, 1, 0, 4b, 0, 1) BYTES(0, 7, 5f, 47, 11, 6c, 75, 61)
                BYTES(5f, 62, 69, 6e, 64, 69, 6e, 67) BYTES(73, 7, 6e, 78, a,
                    70, 61, 69) BYTES(72, 73, 0, 0, 0, 0, 0, 0) 0x0 };
}

