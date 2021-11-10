#include <niu2x/gfx.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <math.h>

#define PI 3.1415926

namespace gfx = nx::gfx;

static gfx::vertex_buffer_t* vbo;
static gfx::vertex_buffer_t* vbo2;
static gfx::indice_buffer_t* ibo;

static auto vertex_layout = gfx::vertex_layout(
    gfx::vertex_attr_type::position, gfx::vertex_attr_type::color);

static gfx::program_t* program = nullptr;

static gfx::render_state_t render_state = gfx::CULL_BACK | gfx::WRITE_G
    | gfx::WRITE_RGBA | gfx::DEPTH_TEST | gfx::WRITE_DEPTH;

static gfx::mat4x4_t view = gfx::mat4x4_identity, model = gfx::mat4x4_identity,
                     projection = gfx::mat4x4_identity;

static const char* vert_shader = R"RAW(
#version 300 es


layout(location = 0) in highp vec3 position;
layout(location = 1) in highp vec4 color;

uniform  mat4 mvp;
out highp vec4 v_color;

void main()
{
  gl_Position = mvp * vec4(position, 1.0);
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
    // clang-format off
    static float vertices[][7] = {
        { 0, 0, 0.5, 1, 0, 1, 1 },
        { 0, 1, 0.5, 1, 1, 0, 1 },
        { 1, 0, 0.5, 0, 1, 1, 1 },
        { 1, 1, 0.5, 1, 1, 1, 1 },
    };

    static float vertices2[][7] = {
        { 0, 0, 0.1, 0, 0, 1, 1 },
        { 0, 1, 0.1, 0, 0, 1, 1 },
        { 1, 0, 0.9, 0, 0, 1, 1 },
        { 1, 1, 0.9, 0, 0, 1, 1 },
    };

    // clang-format on
    static uint32_t indices[] = { 0, 2, 1, 1, 2, 3 };
    gfx::set_clear_color(gfx::rgba(255, 0, 0, 255));
    vbo = gfx::create_vertex_buffer(vertex_layout, 4, vertices);
    vbo2 = gfx::create_vertex_buffer(vertex_layout, 4, vertices2);
    ibo = gfx::create_indice_buffer(6, indices);
    program = gfx::create_program(vert_shader, frag_shader);

    gfx::set_projection_transform(projection);
    gfx::set_view_transform(view);
}

static void cleanup()
{
    gfx::destroy(vbo);
    gfx::destroy(vbo2);
    gfx::destroy(ibo);
    gfx::destroy(program);
}

static void update(double dt)
{
    static double k = 0;
    k += 0.01;
    model = { .data
        = { cos(PI / 6 + k), -sin(PI / 6 + k), 0, 0, sin(PI / 6 + 2 * k),
            cos(PI / 6 + k), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 } };

    gfx::begin();

    gfx::reset();
    gfx::set_render_state(render_state);
    gfx::clear(0);

    gfx::reset();
    gfx::set_model_transform(model);
    gfx::set_render_state(render_state);
    gfx::set_vertex_buffer(vbo);
    gfx::set_indice_buffer(ibo);
    gfx::set_program(program);
    gfx::draw_element(0, 0, 6);

    gfx::set_vertex_buffer(vbo2);
    gfx::draw_element(0, 0, 6);

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