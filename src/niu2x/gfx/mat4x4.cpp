#include "gfx.h"

#include <niu2x/log.h>

namespace nx::gfx {

void mat4x4_dump(const mat4x4 a)
{
    NX_LOG_D("mat4x4 %p: \n", a);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            NX_LOG_D("%f ", a[i][j]);
        }
        NX_LOG_D("\n");
    }
}

} // namespace nx::gfx