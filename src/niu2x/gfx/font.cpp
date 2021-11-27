#include "gfx.h"

#include <ft2build.h>
#include <freetype/freetype.h>
#include <niu2x/log.h>

#define FONT_ALTAS_TEXTURE_CAPACITY 32

namespace nx::gfx {

struct font_altas_t {
    struct texture_t* textures[FONT_ALTAS_TEXTURE_CAPACITY];
};

}; // namespace nx::gfx