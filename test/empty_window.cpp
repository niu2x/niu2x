#include <niu2x/gfx.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <math.h>

#define PI 3.1415926

namespace gfx = nx::gfx;

static gfx::vertex_buffer_t* vbo;
static gfx::indice_buffer_t* ibo;

static auto vertex_layout = gfx::vertex_layout(
    gfx::vertex_attr_type::position, gfx::vertex_attr_type::color);

static gfx::program_t* program = nullptr;

static gfx::render_state_t render_state
    = gfx::WRITE_RGBA | gfx::DEPTH_TEST | gfx::WRITE_DEPTH
    // | gfx::CULL_BACK;
    ;

static gfx::mat4x4 view, model0, model1, projection;

static const char* vert_shader = R"RAW(
#version 300 es


layout(location = 0) in highp vec3 position;
layout(location = 1) in highp vec4 color;

uniform  mat4 mvp;
out highp vec4 v_color;

void main()
{
  gl_Position =  vec4(position, 1.0) * mvp;
  
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
    gfx::mat4x4_identity(model0);
    gfx::mat4x4_identity(model1);
    gfx::mat4x4_identity(projection);
    gfx::mat4x4_identity(view);

    gfx::mat4x4_translate(model0, -0.5, -0.5, -0.5);

    // clang-format off

    float eye[] = {8, 8, 8};
    float center[] = {0.5, 0.5, 0.5};
    float up[] = {0, 0, 1};
    gfx::mat4x4_look_at(view, eye, center, up);

    gfx::mat4x4_perspective(projection, PI*0.06, 1, 0.05, 50);
    // gfx::mat4x4_ortho(projection, -1, 1, -1, 1, 0, 14);

    static float vertices[][7] = {
        { 0, 0, 0, 1, 0, 1, 1 },
        { 1, 0, 0, 0, 1, 1, 1 },
        { 1, 1, 0, 1, 1, 1, 1 },
        { 0, 1, 0, 1, 1, 0, 1 },

        
        { 0, 0, 1, 1, 0, 1, 1 },
        { 1, 0, 1, 0, 1, 1, 1 },
        { 1, 1, 1, 1, 1, 1, 1 },
        { 0, 1, 1, 1, 1, 0, 1 },
    };

    static uint32_t indices[] = { 
        0, 2, 1, 0, 3, 2 ,
        0+4, 2+4, 1+4, 0+4, 3+4, 2 +4,
        0,1,5,  0, 4,5,
        2,3,6, 3, 7,6,
        1,2,5,  2, 6,5,
        3,0,7,  0, 4,7,
    };
    // clang-format on
    gfx::set_clear_color(gfx::rgba(255, 0, 0, 255));
    vbo = gfx::create_vertex_buffer(vertex_layout, 8, vertices);
    ibo = gfx::create_indice_buffer(36, indices);
    program = gfx::create_program(vert_shader, frag_shader);

    gfx::set_projection_transform(projection);
    gfx::set_view_transform(view);
}

static void cleanup()
{
    gfx::destroy(vbo);
    gfx::destroy(ibo);
    gfx::destroy(program);
}

static void update(double dt)
{
    // gfx::mat4x4_rotate_X(model0, model0, 0.01);

    gfx::mat4x4 rotate;

    gfx::mat4x4_rotate_X(rotate, 0.01);
    gfx::mat4x4_mul(model0, model0, rotate);

    gfx::mat4x4_rotate_Y(rotate, 0.015);
    gfx::mat4x4_mul(model0, model0, rotate);

    gfx::mat4x4_rotate_Z(rotate, 0.005);
    gfx::mat4x4_mul(model0, model0, rotate);

    gfx::begin();

    gfx::reset();
    gfx::set_render_state(render_state);
    gfx::clear(0);

    gfx::reset();
    gfx::set_model_transform(model0);
    gfx::set_render_state(render_state);
    gfx::set_vertex_buffer(vbo);
    gfx::set_indice_buffer(ibo);
    gfx::set_program(program);
    gfx::draw_element(0, 0, 36);

    // gfx::mat4x4_rotate_X(model1, model1, 0.02);
    // gfx::set_model_transform(model1);
    // gfx::draw_element(0, 0, 6);

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