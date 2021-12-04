#include <niu2x/gfx.h>
#include <niu2x/log.h>

#include <math.h>

#define PI 3.1415926

namespace gfx = nx::gfx;

static gfx::vertex_buffer_t* vbo;
static gfx::indice_buffer_t* ibo;

static auto vertex_layout
    = gfx::vertex_layout_build(gfx::vertex_attr_type::position,
        gfx::vertex_attr_type::color, gfx::vertex_attr_type::uv);

static gfx::program_t* program = nullptr;

static gfx::render_state_t render_state = gfx::WRITE_RGBA | gfx::DEPTH_TEST
    | gfx::WRITE_DEPTH | gfx::BLEND | gfx::CULL_BACK;
static gfx::texture_t* tex;

static gfx::mat4x4 view, model0, model1, projection;
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
  v_normal = (vec4(normal, 0.0) * transpose(inverse(M))).xyz;
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
    highp vec3 eye = vec3(1000.0, 0, 0);
    highp vec3 light = vec3(10000.0, 10000.0, 20000.0);

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
    gfx::mat4x4_identity(model0);
    gfx::mat4x4_identity(model1);
    gfx::mat4x4_identity(projection);
    gfx::mat4x4_identity(view);

    // gfx::mat4x4_translate(model0, -0.5, -0.5, -0.5);
    // gfx::mat4x4_scale(model1, model1, 0.0000001);
    gfx::mat4x4 rotate;
    gfx::mat4x4_scale_aniso(model0, model0, 1, 1, 1);
    gfx::mat4x4_rotate_X(rotate, 3.1514 / 2);
    gfx::mat4x4_mul(model0, model0, rotate);

    // clang-format off

    float eye[] = {1000, 0, 100};
    float center[] = {0, 0, 10};
    float up[] = {0, 0, 1};
    gfx::mat4x4_look_at(view, eye, center, up);

    gfx::mat4x4_perspective(projection, PI*0.6, 1, 0.05, 5000);
    // gfx::mat4x4_ortho(projection, -1, 1, -1, 1, 0, 14);

    static float vertices[][10] = {
        { 0, 0, 0, 1, 0, 1, 1 ,0,0,0,},
        { 1, 0, 0, 0, 1, 1, 1 ,1,0,0,},
        { 1, 1, 0, 1, 1, 1, 1 ,1,1,0,},
        { 0, 1, 0, 1, 1, 0, 1 ,0,1,0,},
        { 0, 0, 1, 1, 0, 1, 1 ,0,1,0,},
        { 1, 0, 1, 0, 1, 1, 1 ,1,1,0,},
        { 1, 1, 1, 1, 1, 1, 1 ,1,0,0,},
        { 0, 1, 1, 1, 1, 0, 1 ,0,0,0,},
    };

    static uint32_t indices[] = { 
        0, 2, 1, 0, 3, 2 ,
        2+4, 0+4, 1+4, 3+4, 0+4,  2 +4,
        0,1,5,  0, 4,5,
        2,3,6, 3, 7,6,
        1,2,5,  2, 6,5,
        7,3,0, 0,4,7,
    };
    // clang-format on
    gfx::set_clear_color(gfx::rgba(255, 0, 0, 255));
    vbo = gfx::create_vertex_buffer(vertex_layout, 8, vertices);
    ibo = gfx::create_indice_buffer(36, indices);
    program = gfx::create_program(vert_shader, frag_shader);

    gfx::set_projection_transform(projection);
    gfx::set_view_transform(view);

    tex = gfx::create_texture_2d_from_file("../test/mu-wan-qing.jpeg");
    mesh = gfx::create_mesh_from_file(
        "../test/Aya.obj", 0, gfx::MESH_AUTO_CENTER);
    mesh->texture = gfx::create_texture_2d_from_file("../test/Aya.jpg");
}

static void cleanup()
{
    gfx::destroy(mesh);
    gfx::destroy(tex);
    gfx::destroy(vbo);
    gfx::destroy(ibo);
    gfx::destroy(program);
}

static void update(double dt)
{
    // gfx::mat4x4_rotate_X(model0, model0, 0.01);

    gfx::mat4x4 rotate;

    // gfx::mat4x4_rotate_X(rotate, 0.01);
    // gfx::mat4x4_mul(model0, model0, rotate);

    gfx::mat4x4_rotate_Z(rotate, 0.015);
    gfx::mat4x4_mul(model0, model0, rotate);

    gfx::begin();

    gfx::reset();
    gfx::set_render_state(render_state);
    gfx::clear(0);

    gfx::reset();
    gfx::set_model_transform(model0);
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