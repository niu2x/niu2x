#include "gfx.h"

namespace nx::gfx {

void setup()
{

    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    font::font_system_setup();
}
void cleanup() { font::font_system_cleanup(); }
} // namespace nx::gfx