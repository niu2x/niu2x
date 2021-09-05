#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#define NX_LOG_LEVEL 5

#include <niu2x/memory.h>
#include <niu2x/io.h>
#include <niu2x/aio.h>
#include <niu2x/gfx.h>
#include <niu2x/lexical_cast.h>

using namespace nx;

static void update(double dt)
{
    NX_LOG_D("update interval: %f fps:%f", dt, 1.0 / dt);
}

int main()
{
    gfx::config config {
        .title = "nx window",
        .width = 800,
        .height = 600,
        .options = gfx::default_option,
        .update = update,
    };
    gfx::run(config);
    return 0;
}