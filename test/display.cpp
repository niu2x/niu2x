#include "cxxopts.h"

#include <niu2x/gfx.h>
#include <niu2x/log.h>

#include <math.h>

#define PI 3.1415926

namespace gfx = nx::gfx;

static gfx::program_t* program = nullptr;

static gfx::render_state_t render_state
    = gfx::WRITE_RGBA | gfx::DEPTH_TEST | gfx::WRITE_DEPTH | gfx::BLEND;
static gfx::texture_t* tex;

static std::string model_file;
static std::string texture_file;
static double scale = 1;
static bool scale_mode = false;

static gfx::mat4x4 view, model, projection;
static gfx::mesh_t* mesh = nullptr;
static const char* vert_shader = R"RAW(
#version 300 es


layout(location = 0) in highp vec3 position;
layout(location = 1) in highp vec3 normal;
layout(location = 2) in highp vec4 uv;

uniform mat4 MVP;
uniform mat4 M;

out highp vec3 v_normal;
out highp vec3 v_world_pos;
out highp vec4 v_uv;

void main()
{
  v_normal = normalize((vec4(normal, 0.0) * transpose(inverse(M))).xyz);
  gl_Position =  vec4(position, 1.0) * MVP;

  highp vec4 world_pos = vec4(position, 1.0) * M;
  v_uv = uv;
  v_world_pos = world_pos.xyz / world_pos.w;
}

)RAW";

static const char* frag_shader = R"RAW(
#version 300 es

uniform sampler2D TEX0;

in highp vec3 v_normal;
in highp vec3 v_world_pos;
in highp vec4 v_uv;

out highp vec4 color;

void main()
{
    highp vec3 eye = vec3(100.0, 0, 0.0);
    highp vec3 light = vec3(10000.0, 0, 20000.0);

    highp vec3 material = texture(TEX0, v_uv.xy).xyz;
    highp vec3 ambient = vec3(0.7);
    highp vec3 l = normalize(light - v_world_pos);
    highp vec3 v = normalize(eye - v_world_pos);

    highp vec3 diffuse = max(dot(v_normal, l), 0.0) * vec3(0.5);
    highp vec3 mirror = pow(max(dot(normalize(v+l), v_normal), 0.0), 32.0) * vec3(1.0);

    color = vec4(material * (diffuse + ambient ) + mirror, 1.0);
}

)RAW";

static void setup()
{
    gfx::mat4x4_identity(model);
    gfx::mat4x4_identity(projection);
    gfx::mat4x4_identity(view);

    gfx::mat4x4 rotate;
    gfx::mat4x4_scale_aniso(model, model, 1, 1, 1);
    gfx::mat4x4_rotate_X(rotate, PI / 2);
    gfx::mat4x4_mul(model, model, rotate);

    float eye[] = { 100, 0, 0 };
    float center[] = { 0, 0, 10 };
    float up[] = { 0, 0, 1 };
    gfx::mat4x4_look_at(view, eye, center, up);

    gfx::mat4x4_perspective(projection, PI * 0.6, 1, 0.05, 5000);

    gfx::set_clear_color(gfx::rgba(32, 32, 32, 0));

    program = gfx::create_program(vert_shader, frag_shader);

    gfx::set_projection_transform(projection);
    gfx::set_view_transform(view);

    mesh = gfx::create_mesh_from_file(
        model_file.c_str(), 0, gfx::MESH_AUTO_CENTER);
    mesh->texture = gfx::create_texture_2d_from_file(texture_file.c_str());
}

static void cleanup()
{
    gfx::destroy(mesh);
    gfx::destroy(program);
}

static void update(double dt)
{

    gfx::mat4x4 rotate;

    gfx::mat4x4_rotate_Z(rotate, 0.015);
    gfx::mat4x4_mul(model, model, rotate);

    gfx::mat4x4 this_model;
    gfx::mat4x4_scale_aniso(this_model, model, scale, scale, scale);

    gfx::begin();

    gfx::reset();
    gfx::set_render_state(render_state);
    gfx::clear(0);

    gfx::set_model_transform(this_model);
    gfx::set_render_state(render_state);
    gfx::set_program(program);

    gfx::set_texture(0, mesh->texture);
    gfx::set_blend_func(gfx::blend::src_color, gfx::blend::one_minus_src_alpha);
    gfx::set_vertex_buffer(mesh->vb);
    gfx::set_indice_buffer(mesh->ib);

    gfx::draw_element(0, 0, mesh->ib->size);

    gfx::end();
}

static void key_callback(int keycode, int action, int mods)
{
    if (keycode == gfx::KEY_ESCAPE && action == gfx::KEY_RELEASE) {
        gfx::exit();
    }

    if (keycode == gfx::KEY_S) {
        scale_mode = action != gfx::KEY_RELEASE;
        NX_LOG_D("scale_mode %d", scale_mode);
    }
}

static double posx = -1, posy = -1;

static void mouse_pos_callback(double xpos, double ypos)
{
    if (posx >= 0 && posy >= 0) {
        if (scale_mode) {
            scale += (ypos - posy) / 100;
            scale = std::max(scale, 0.5);
            scale = std::min(scale, 100.0);
        }
    }

    posx = xpos;
    posy = ypos;
}

int main(int argc, char* argv[])
{
    cxxopts::Options options("display", "display 3d model");

    options.add_options()(
        "m,model", "model file", cxxopts::value<std::string>())(
        "t,texture", "texture", cxxopts::value<std::string>());
    auto result = options.parse(argc, argv);

    model_file = result["model"].as<std::string>();
    texture_file = result["texture"].as<std::string>();

    gfx::window_config cfg;
    cfg.title = "display";
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