#include <niu2x/gfx.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace gfx = nx::gfx;

static gfx::vertex_buffer_t* vbo;

static void setup()
{
    gfx::set_clear_color(gfx::rgba(255, 0, 0, 255));
    vbo = gfx::create_vertex_buffer();
}

static void cleanup() { gfx::destroy(vbo); }

static void update(double dt)
{
    gfx::begin();
    gfx::clear(0);
    gfx::end();
}

int main()
{
    gfx::window_config cfg;
    cfg.title = "empty window";
    cfg.options = gfx::MSAA;
    cfg.width = 800;
    cfg.height = 600;
    cfg.setup = setup;
    cfg.cleanup = cleanup;
    cfg.update = update;

    gfx::run(cfg);
    return 0;
}