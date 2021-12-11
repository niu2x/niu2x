#include "gfx.h"

namespace nx::gfx {

program_t* program_create(const char* vert_frag[])
{
    return program_create(vert_frag[0], vert_frag[1]);
}
} // namespace nx::gfx
