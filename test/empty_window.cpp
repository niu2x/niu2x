#include <niu2x/gfx.h>
#include <niu2x/log.h>

#include <math.h>

namespace gfx = nx::gfx;
namespace math = nx::math;

static void setup() { }

static void cleanup() { }

static void update(double dt)
{
    gfx::begin();
    gfx::set_render_state(gfx::WRITE_RGBA);
    gfx::clear(0);
    gfx::end();
}

static void key_callback(int keycode, int action, int mods)
{
    if (keycode == gfx::KEY_ESCAPE && action == gfx::KEY_RELEASE) {
        gfx::exit();
    }
}

static void mouse_pos_callback(double xpos, double ypos)
{
    // NX_LOG_D("mouse pos: %f %f", xpos, ypos);
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
    cfg.key_callback = key_callback;
    cfg.mouse_pos_callback = mouse_pos_callback;

    gfx::run(cfg);
    return 0;
}