#ifndef NX_GFX_GFX_H
#define NX_GFX_GFX_H

#include <niu2x/gfx.h>

namespace nx::gfx {

using indice_t = uint32_t;

size_t vertex_sizeof(vertex_layout_t layout);

GLint program_uniform_location(struct program_t* obj, const char* name);

#define NX_CHECK_GL_ERROR()                                                    \
    do {                                                                       \
        auto e = glGetError();                                                 \
        NX_ASSERT(e == GL_NO_ERROR, "opengl error: %x", e);                    \
    } while (false);

} // namespace nx::gfx

void setup();
void cleanup();

#endif