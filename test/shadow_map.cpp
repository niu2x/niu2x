#include "cxxopts.h"

#include <niu2x/gfx.h>
#include <niu2x/log.h>

#include <math.h>

#define PI 3.1415926

namespace gfx = nx::gfx;
namespace math = nx::math;

static gfx::program_t* program = nullptr;
static gfx::program_t* program000 = nullptr;
static gfx::program_t* ambient_program = nullptr;

static gfx::render_state_t render_state
    = gfx::WRITE_RGBA | gfx::DEPTH_TEST | gfx::WRITE_DEPTH | gfx::BLEND;
static gfx::texture_t* tex;

static std::string model_file;
static std::string texture_file;
static double scale = 1;
static bool scale_mode = false;

static gfx::vertex_buffer_t* floor_vb;
static gfx::indice_buffer_t* floor_ib;

static gfx::mat4x4 view, model, projection;
static gfx::mesh_t* mesh = nullptr;

static const char* vert_shader = R"RAW(
#version 300 es


layout(location = 0) in highp vec3 position;
layout(location = 1) in highp vec3 normal;
layout(location = 2) in highp vec3 uv;

uniform mat4 MVP;
uniform mat4 M;

out highp vec3 v_normal;
out highp vec3 v_world_pos;
out highp vec3 v_uv;

void main()
{
  v_normal = normalize((vec4(normal, 0.0) * transpose(inverse(M))).xyz);
  gl_Position =  vec4(position, 1.0) * MVP;

  highp vec4 world_pos = vec4(position, 1.0) * M;
  v_uv = uv;
  v_world_pos = world_pos.xyz / world_pos.w;
}

)RAW";

static const char* ambient_frag_shader = R"RAW(
#version 300 es

uniform sampler2D TEX0;

in highp vec3 v_normal;
in highp vec3 v_world_pos;
in highp vec3 v_uv;

out highp vec4 color;

uniform highp mat4 V;
uniform highp float TIME;

void main()
{
    highp vec3 eye = vec3(100.0, 0, 50.0);
    highp vec3 light = vec3(-100.0, 0, 100.0);

    highp vec3 ambient = vec3(0.7);
    highp vec3 l = normalize(light - v_world_pos);
    highp vec3 v = normalize(eye - v_world_pos);

    highp vec3 diffuse = max(dot(v_normal, l), 0.0) * vec3(0.5);
    highp vec3 mirror = pow(max(dot(normalize(v+l), v_normal), 0.0), 32.0) * vec3(1.0);

    color = vec4(0.1, 0.1, 0.1, 1.0) * vec4(( ambient ) , 1.0);
}

)RAW";

static const char* ambient_vert_shader = R"RAW(
#version 300 es


layout(location = 0) in highp vec3 position;
layout(location = 1) in highp vec3 normal;
layout(location = 2) in highp vec3 uv;

uniform mat4 MVP;
uniform mat4 M;

out highp vec3 v_normal;
out highp vec3 v_world_pos;
out highp vec3 v_uv;

void main()
{
  v_normal = normalize((vec4(normal, 0.0) * transpose(inverse(M))).xyz);
  gl_Position =  vec4(position, 1.0) * MVP;

  highp vec4 world_pos = vec4(position, 1.0) * M;
  v_uv = uv;
  v_world_pos = world_pos.xyz / world_pos.w;
}

)RAW";

static const char* frag_shader000 = R"RAW(
#version 300 es

uniform sampler2D TEX0;

in highp vec3 v_normal;
in highp vec3 v_world_pos;
in highp vec3 v_uv;

out highp vec4 color;

uniform highp mat4 V;
uniform highp float TIME;

void main()
{
    highp vec3 eye = vec3(100.0, 0, 50.0);
    highp vec3 light = vec3(-100.0, 0, 100.0);

    highp vec3 material = texture(TEX0, v_uv.xy).xyz;

    highp vec3 ambient = vec3(0.7);
    highp vec3 l = normalize(light - v_world_pos);
    highp vec3 v = normalize(eye - v_world_pos);

    highp vec3 diffuse = max(dot(v_normal, l), 0.0) * vec3(0.5);
    highp vec3 mirror = pow(max(dot(normalize(v+l), v_normal), 0.0), 32.0) * vec3(1.0);

    color = vec4(material * (diffuse + ambient ) + mirror, 1.0);

}

)RAW";

static const char* frag_shader = R"RAW(
#version 300 es

uniform sampler2D TEX0;
uniform sampler2D TEX1;

in highp vec3 v_normal;
in highp vec3 v_world_pos;
in highp vec3 v_uv;

out highp vec4 color;

uniform highp mat4 V;
uniform highp float TIME;

highp mat4 ortho(highp float l, highp float r, highp float b, highp float t, highp float n, highp float f){
    highp mat4 M;
    M[0][0] = 2.0 / (r - l);
    M[1][0] = M[2][0] = M[3][0] = 0.0;

    M[1][1] = 2.0 / (t - b);
    M[0][1] = M[2][1] = M[3][1] = 0.0;

    M[2][2] = -2.0 / (f - n);
    M[0][2] = M[1][2] = M[3][2] = 0.0;

    M[0][3] = -(r + l) / (r - l);
    M[1][3] = -(t + b) / (t - b);
    M[2][3] = -(f + n) / (f - n);
    M[3][3] = 1.0;
    return M;
}

highp mat4 look_at(highp vec3 eye, highp vec3 center, highp vec3 up) {

    highp vec3 f = normalize(center - eye);
    highp vec3 s = normalize(cross(f, up));
    highp vec3 t = cross(s, f);

    highp mat4 temp;

    temp[0][0] = s[0];
    temp[1][0] = t[0];
    temp[2][0] = -f[0];
    temp[3][0] = 0.0;

    temp[0][1] = s[1];
    temp[1][1] = t[1];
    temp[2][1] = -f[1];
    temp[3][1] = 0.0;

    temp[0][2] = s[2];
    temp[1][2] = t[2];
    temp[2][2] = -f[2];
    temp[3][2] = 0.0;

    temp[0][3] = 0.0;
    temp[1][3] = 0.0;
    temp[2][3] = 0.0;
    temp[3][3] = 1.0;
    highp mat4 translate = mat4(
        1.0, 0.0, 0.0, -eye[0]
        , 0.0, 1.0, 0.0, -eye[1]
        , 0.0, 0.0, 1.0, -eye[2]
        , 0.0, 0.0, 0.0, 1.0
    );

    return translate * temp;
}

void main()
{
    highp vec3 eye = vec3(100.0, 0, 50.0);
    highp vec3 light = vec3(-100.0, 0, 100.0);


    highp vec3 ambient = vec3(0.3);
    highp vec3 l = normalize(light - v_world_pos);
    highp vec3 v = normalize(eye - v_world_pos);

    highp vec3 diffuse = max(dot(v_normal, l), 0.0) * vec3(0.5);
    highp vec3 mirror = pow(max(dot(normalize(v+l), v_normal), 0.0), 32.0) * vec3(1.0);

    color = vec4((diffuse + ambient ) + mirror, 1.0);

    highp mat4 vv = look_at(light, vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 1.0));
    highp mat4 vvvv = ortho(-256.0/2.0, 256.0/2.0, -256.0/2.0, 256.0/2.0, 0.0, 1000000.0);
    highp vec4 pos_at_light  = vec4(v_world_pos, 1.0) * vv * vvvv; 
    pos_at_light.xyz /= pos_at_light.w;

  //  highp vec4 shadow = textureGrad(TEX1, vec2(0.5) + pos_at_light.xy*0.5, vec2(0.0), vec2(0.0));
    highp vec4 shadow = texture(TEX1, vec2(0.5) + pos_at_light.xy*0.5);
    color.a = 1.0-shadow.a;


}

)RAW";

static void setup()
{
    math::mat4x4_identity(model);
    math::mat4x4_identity(projection);
    math::mat4x4_identity(view);

    math::mat4x4_scale_aniso(model, 0.1, 0.1, 0.1);
    math::mat4x4_rotate_x(model, PI / 2);
    math::mat4x4_translate(model, 0, 0, 50);

    float eye[] = { 100, 0, 50 };
    float center[] = { 0, 0, 0 };
    float up[] = { 0, 0, 1 };
    math::mat4x4_look_at(view, eye, center, up);

    math::mat4x4_perspective(projection, PI * 0.6, 1, 0.05, 5000);

    gfx::set_clear_color(gfx::rgba(0, 0, 0, 0));

    program = gfx::program_create(vert_shader, frag_shader);
    program000 = gfx::program_create(vert_shader, frag_shader000);
    ambient_program
        = gfx::program_create(ambient_vert_shader, ambient_frag_shader);

    gfx::set_projection_transform(projection);
    gfx::set_view_transform(view);

    mesh = gfx::mesh_create_from_file(
        model_file.c_str(), 0, gfx::MESH_AUTO_CENTER);
    mesh->texture = gfx::texture_create_from_file(texture_file.c_str());

    tex = gfx::texture_create(2048, 2048, gfx::pixel_format::rgba8, 0);

    float floor_vertices[][9] = {
        { -100000, -100000, -0, 0, 0, 1, 0, 0, 0 },
        { -100000, 100000, -0, 0, 0, 1, 0, 0, 0 },
        { 100000, 100000, -0, 0, 0, 1, 0, 0, 0 },
        { 100000, -100000, -0, 0, 0, 1, 0, 0, 0 },
    };

    floor_vb = gfx::vertex_buffer_create(
        gfx::vertex_layout(gfx::vertex_attr::position, gfx::vertex_attr::normal,
            gfx::vertex_attr::uv),
        4, floor_vertices);

    uint32_t floor_indices[] = { 0, 1, 2, 0, 2, 3 };

    floor_ib = gfx::indice_buffer_create(6, floor_indices);
}

static void cleanup()
{
    gfx::destroy(ambient_program);
    gfx::destroy(floor_ib);
    gfx::destroy(floor_vb);
    gfx::destroy(tex);
    gfx::destroy(mesh);
    gfx::destroy(program);
}

static void update(double dt)
{

    math::mat4x4_rotate_z(model, 0.015);
    gfx::mat4x4 this_model;
    math::mat4x4_dup(this_model, model);
    math::mat4x4_scale_aniso(this_model, scale, scale, scale);

    gfx::begin();
    gfx::set_view(1, tex);

    gfx::reset();
    gfx::set_render_state(render_state);
    gfx::clear(0);
    gfx::clear(1);

    gfx::set_render_state(render_state & (~gfx::DEPTH_TEST));
    gfx::set_program(ambient_program);

    gfx::set_blend_func(gfx::blend::src_alpha, gfx::blend::one_minus_src_alpha);

    gfx::set_model_transform(math::identity_mat4x4);
    gfx::set_vertex_buffer(floor_vb);
    gfx::set_indice_buffer(floor_ib);
    gfx::draw_element(0, 0, 6);

    gfx::begin();
    {
        math::mat4x4 look_at_light;
        math::vec3 eye = { -100.0, 0, 100.0 };
        math::vec3 center = { 0, 0, 50 };
        math::vec3 up = { 0, 0, 1 };
        math::mat4x4_look_at(look_at_light, eye, center, up);
        gfx::set_view_transform(look_at_light);

        math::mat4x4 projection_at_light;
        int w = 256, h = 256;
        math::mat4x4_ortho(
            projection_at_light, -w / 2, w / 2, -h / 2, h / 2, 0, 1000000);
        gfx::set_projection_transform(projection_at_light);

        gfx::set_blend_func(
            gfx::blend::src_alpha, gfx::blend::one_minus_src_alpha);
        gfx::set_render_state((render_state) & (~gfx::DEPTH_TEST));
        gfx::set_program(ambient_program);
        gfx::set_model_transform(this_model);
        // gfx::set_vertex_buffer(mesh->vb);
        // gfx::set_indice_buffer(mesh->ib);
        // gfx::draw_element(1, 0, mesh->ib->size);
        gfx::draw_mesh(1, mesh);
    }
    gfx::end();

    gfx::begin();
    {
        gfx::set_texture(1, tex);
        gfx::set_blend_func(
            gfx::blend::src_alpha, gfx::blend::one_minus_src_alpha);
        gfx::set_render_state((render_state) & (~gfx::DEPTH_TEST));
        gfx::set_program(program);
        gfx::set_model_transform(math::identity_mat4x4);
        gfx::set_vertex_buffer(floor_vb);
        gfx::set_indice_buffer(floor_ib);
        gfx::draw_element(0, 0, 6);
    }
    gfx::end();

    gfx::set_render_state((render_state));
    gfx::set_program(program000);
    gfx::set_model_transform(this_model);
    gfx::draw_mesh(0, mesh);

    // gfx::draw_texture(0, tex);

    gfx::end();
}

static void key_callback(int keycode, int action, int mods)
{
    if (keycode == gfx::KEY_ESCAPE && action == gfx::KEY_RELEASE) {
        gfx::exit();
    }

    if (keycode == gfx::KEY_S) {
        scale_mode = action != gfx::KEY_RELEASE;
    }
}

static double posx = -1, posy = -1;

static void mouse_pos_callback(double xpos, double ypos)
{
    if (posx >= 0 && posy >= 0) {
        if (scale_mode) {
            scale += (ypos - posy) / 10;
            scale = std::max(scale, 0.01);
            scale = std::min(scale, 100.0);
        }
    }

    posx = xpos;
    posy = ypos;
}

int main(int argc, char* argv[])
{
    // cxxopts::Options options("shadow_map", "display 3d model");

    // options.add_options()(
    //     "m,model", "model file", cxxopts::value<std::string>())(
    //     "t,texture", "texture", cxxopts::value<std::string>());
    // auto result = options.parse(argc, argv);

    model_file = "../test/Aya.obj";
    texture_file = "../test/Aya.jpg";

    gfx::window_config cfg;
    cfg.title = "shadow map";
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