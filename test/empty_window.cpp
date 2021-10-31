#include <niu2x/gfx.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

static void update(double dt) { glClear(GL_COLOR_BUFFER_BIT); }

int main()
{
    nx::gfx::window_config cfg;
    cfg.title = "empty window";
    cfg.options = nx::gfx::MSAA;
    cfg.width = 800;
    cfg.height = 600;
    cfg.setup = nullptr;
    cfg.cleanup = nullptr;
    cfg.update = update;

    nx::gfx::run(cfg);
    return 0;
}