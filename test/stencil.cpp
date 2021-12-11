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

static const char* bdsf_vert = R"RAW(
#version 300 es
precision highp float;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 uv;

out vec4 v_world_pos;
out vec3 v_normal;
out vec3 v_uv;

uniform mat4 MVP;
uniform mat4 M;

void main()
{
  gl_Position =  vec4(position, 1.0) * MVP;
  v_world_pos = vec4(position, 1.0) * M;
  v_normal = (vec4(normal, 0.0) * transpose(inverse(M))).xyz;
  v_uv = uv;
}

)RAW";

static const char* bdsf_frag = R"RAW(
#version 300 es
precision highp float;

out vec4 color;

uniform sampler2D TEX0;

in vec4 v_world_pos;
in vec3 v_normal;
in vec3 v_uv;

void main()
{
    vec3 eye = vec3(100.0);

    vec3 material = texture(TEX0, v_uv.xy).rgb;
    vec3 normal = normalize(v_normal);
    vec3 l = normalize(vec3(1.0, 1.0, 1.0));
    vec3 v = normalize(eye - v_world_pos.xyz/v_world_pos.w);

    vec3 spec = vec3(0.2) * pow(max(0.0, dot(normalize((v+l)*0.5), normal)), 16.0);

    float diffuse = max(dot(normal, l), 0.0)*0.5;
    color = vec4( material*(0.3 + diffuse) + spec, 1.0);

}

)RAW";

static gfx::texture_t* floor_tex;
static gfx::mesh_t* mesh;
static gfx::program_t* program_ambient;
static gfx::program_t* program_bdsf;
static math::mat4x4 model, projection, view;
static math::mat4x4 floor_model, shadow_model;

static gfx::vertex_buffer_t* floor_vb;
static gfx::indice_buffer_t* floor_ib;

static void shadow_transform(math::mat4x4 k)
{
    math::mat4x4 mul;
    mul[0][0] = 1;
    mul[1][0] = 0;
    mul[2][0] = -0.6;
    mul[3][0] = 0;

    mul[0][1] = 0;
    mul[1][1] = 1;
    mul[2][1] = -0.6;
    mul[3][1] = 0;

    mul[0][2] = 0;
    mul[1][2] = 0;
    mul[2][2] = 0;
    mul[3][2] = 0;

    mul[0][3] = 0;
    mul[1][3] = 0;
    mul[2][3] = 0;
    mul[3][3] = 1;

    math::mat4x4_mul(k, mul);
}

static void setup()
{
    mesh = gfx::mesh_create_from_file(
        "../test/Aya.obj", 0, gfx::MESH_AUTO_CENTER);
    mesh->texture = gfx::texture_create_from_file("../test/Aya.jpg");

    program_ambient = gfx::program_create(ambient_vert, ambient_frag);
    program_bdsf = gfx::program_create(bdsf_vert, bdsf_frag);

    gfx::set_clear_color(gfx::rgba(0, 0, 0, 0));

    math::mat4x4_identity(model);
    math::mat4x4_identity(view);
    math::mat4x4_identity(projection);

    math::mat4x4_scale_aniso(model, 0.1, 0.1, 0.1);
    math::mat4x4_rotate_x(model, 3.1415 / 2);
    math::mat4x4_translate(model, 0, 0, 68);

    math::vec3 eye { 100, 100, 51 };
    math::vec3 center { 0, 0, 0 };
    math::vec3 up { 0, 0, 1 };
    math::mat4x4_look_at(view, eye, center, up);

    math::mat4x4_perspective(projection, 3.14 * 0.6, 1.0, 0.1, 10000.0);

    gfx::set_view_transform(view);
    gfx::set_projection_transform(projection);

    float floor_vertices[][9] = {
        {
            -200,
            -200,
            -0,
            0,
            0,
            1,
            0,
            0,
            0,
        },
        { -200, 200, -0, 0, 0, 1, 0, 1, 0 },
        { 200, 200, -0, 0, 0, 1, 1, 1, 0 },
        { 200, -200, -0, 0, 0, 1, 1, 0, 0 },
    };

    auto vertex_layout = gfx::vertex_layout(gfx::vertex_attr::position,
        gfx::vertex_attr::normal, gfx::vertex_attr::uv);
    floor_vb = gfx::vertex_buffer_create(vertex_layout, 4, floor_vertices);

    uint32_t floor_indices[] = { 0, 1, 2, 0, 2, 3 };

    floor_ib = gfx::indice_buffer_create(6, floor_indices);

    math::mat4x4_identity(floor_model);

    math::mat4x4_dup(shadow_model, model);
    shadow_transform(shadow_model);

    floor_tex = gfx::texture_create_from_file("../test/floor-stone.jpg");
}
static void cleanup()
{
    destroy(floor_tex);
    destroy(program_bdsf);
    destroy(floor_vb);
    destroy(floor_ib);
    destroy(program_ambient);
    destroy(mesh);
}
static void update(double dt)
{
    math::mat4x4_rotate_z(model, 0.01);
    math::mat4x4_dup(shadow_model, model);
    shadow_transform(shadow_model);

    gfx::begin();
    gfx::set_render_state(
        gfx::WRITE_DEPTH | gfx::WRITE_STENCIL | gfx::WRITE_RGBA);
    gfx::set_stencil_func(gfx::comparator::always, 1, 0xFF);

    gfx::clear(0);

    gfx::set_render_state(gfx::WRITE_STENCIL | gfx::STENCIL_TEST);

    gfx::set_stencil_op(
        gfx::stencil_op::keep, gfx::stencil_op::keep, gfx::stencil_op::replace);
    gfx::clear(0);

    gfx::set_program(program_ambient);
    gfx::set_model_transform(shadow_model);
    gfx::draw_mesh(0, mesh);

    gfx::set_stencil_op(
        gfx::stencil_op::keep, gfx::stencil_op::keep, gfx::stencil_op::keep);
    gfx::set_render_state(gfx::WRITE_RGBA);

    gfx::set_model_transform(floor_model);
    gfx::set_vertex_buffer(floor_vb);
    gfx::set_indice_buffer(floor_ib);
    gfx::draw_element(0, 0, 6);

    gfx::set_render_state(gfx::WRITE_RGBA | gfx::WRITE_DEPTH | gfx::DEPTH_TEST
        | gfx::STENCIL_TEST);
    gfx::set_stencil_func(gfx::comparator::not_equal, 1, 0xFF);
    gfx::set_program(program_bdsf);
    gfx::set_model_transform(floor_model);
    gfx::set_vertex_buffer(floor_vb);
    gfx::set_indice_buffer(floor_ib);
    gfx::set_texture(0, floor_tex);
    gfx::draw_element(0, 0, 6);

    gfx::set_render_state(gfx::WRITE_RGBA | gfx::WRITE_DEPTH | gfx::DEPTH_TEST);
    gfx::set_model_transform(model);
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