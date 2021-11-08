#ifndef NX_GFX_GFX_H
#define NX_GFX_GFX_H

#include <niu2x/gfx.h>

namespace nx::gfx {

using indice_t = uint32_t;

size_t vertex_sizeof(vertex_layout_t layout);

} // namespace nx::gfx

#endif