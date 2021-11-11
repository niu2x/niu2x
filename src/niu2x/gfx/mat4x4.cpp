#include "gfx.h"

namespace nx::gfx {

void mat4x4_dump(const mat4x4 a)
{
    printf("mat4x4 %p: \n", a);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%f ", a[i][j]);
        }
        printf("\n");
    }
}

} // namespace nx::gfx