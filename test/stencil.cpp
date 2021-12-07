#include <niu2x/gfx.h>

namespace gfx = nx::gfx;
namespace math = nx::math;

static const char* ambient_vert = R"RAW(
#version 300 es
precision highp float;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 uv;

uniform mat4 MVP;

void main()
{
  gl_Position =  vec4(position, 1.0) * MVP;
}

)RAW";

static const char* ambient_frag = R"RAW(
#version 300 es
precision highp float;

out vec4 color;

void main()
{
    color = vec4(vec3(0.2), 1.0);
}

)RAW";

static gfx::mesh_t* mesh;
static gfx::program_t* program_ambient;
static math::mat4x4 model, projection, view;

static void setup()
{
    mesh = gfx::create_mesh_from_file(
        "../test/Aya.obj", 0, gfx::MESH_AUTO_CENTER);
    mesh->texture = gfx::create_texture_2d_from_file("../test/Aya.jpg");

    program_ambient = gfx::create_program(ambient_vert, ambient_frag);

    gfx::set_clear_color(gfx::rgba(0, 0, 0, 0));

    math::mat4x4_identity(model);
    math::mat4x4_identity(view);
    math::mat4x4_identity(projection);

    math::mat4x4_scale_aniso(model, 0.1, 0.1, 0.1);

    math::vec3 eye { 100, 100, 0 };
    math::vec3 center { 0, 0, 0 };
    math::vec3 up { 0, 0, 1 };
    math::mat4x4_look_at(view, eye, center, up);

    math::mat4x4_perspective(projection, 3.14 * 0.6, 1.0, 0.1, 10000.0);

    gfx::set_view_transform(view);
    gfx::set_projection_transform(projection);
}
static void cleanup()
{
    destroy(program_ambient);
    destroy(mesh);
}
static void update(double dt)
{
    gfx::begin();
    gfx::set_render_state(gfx::WRITE_RGBA | gfx::WRITE_DEPTH);
    gfx::clear(0);
    gfx::set_model_transform(model);
    gfx::set_program(program_ambient);
    gfx::draw_mesh(0, mesh);
    gfx::end();
}

int main()
{

    gfx::window_config cfg;
    cfg.title = "stencil";
    cfg.options = gfx::MSAA;
    cfg.width = 800;
    cfg.height = 600;
    cfg.setup = setup;
    cfg.cleanup = cleanup;
    cfg.update = update;
    cfg.key_callback = nullptr;
    cfg.mouse_pos_callback = nullptr;

    gfx::run(cfg);
    return 0;
}