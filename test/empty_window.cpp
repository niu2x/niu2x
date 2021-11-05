#include <niu2x/gfx.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace gfx = nx::gfx;

static gfx::vertex_buffer_t* vbo;
static auto vertex_layout = gfx::vertex_layout(
    gfx::vertex_attr_type::position, gfx::vertex_attr_type::color);
static gfx::program_t* program = nullptr;

static const char* vert_shader = R"RAW(
#version 300 es

layout(location = 0) in highp vec3 position;
layout(location = 1) in highp vec4 color;

out highp vec4 v_color;

void main()
{
  gl_Position = vec4(position, 1.0);
  v_color = color;
}

)RAW";

static const char* frag_shader = R"RAW(
#version 300 es

in highp vec4 v_color;
out highp vec4 color;

void main()
{
  color = v_color;
}

)RAW";

static void setup()
{
    static float vertices[] = {
        0,
        0,
        0,

        1,
        1,
        1,
        1,

        0,
        1,
        0,

        1,
        1,
        1,
        1,

        1,
        0,
        0,

        1,
        0,
        1,
        1,
    };
    gfx::set_clear_color(gfx::rgba(255, 0, 0, 255));
    vbo = gfx::create_vertex_buffer(vertex_layout, 3, vertices);
    program = gfx::create_program(vert_shader, frag_shader);
}

static void cleanup()
{
    gfx::destroy(vbo);
    gfx::destroy(program);
}

static void update(double dt)
{
    gfx::begin();
    gfx::clear(0);
    gfx::set_vertex_buffer(vbo);
    gfx::set_program(program);
    gfx::draw_array(0, 0, 3);
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