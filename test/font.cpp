#include <niu2x/gfx.h>
#include <niu2x/log.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <math.h>

#define PI 3.1415926

namespace gfx = nx::gfx;

static gfx::vertex_buffer_t* vbo;
static gfx::indice_buffer_t* ibo;

static auto vertex_layout = gfx::vertex_layout(gfx::vertex_attr_type::position,
    gfx::vertex_attr_type::color, gfx::vertex_attr_type::uv);

static gfx::program_t* program = nullptr;

static gfx::render_state_t render_state
    = gfx::WRITE_RGBA | gfx::DEPTH_TEST | gfx::WRITE_DEPTH | gfx::BLEND;
static gfx::font_t* font;

static gfx::mat4x4 view, model0, projection;

static const char* vert_shader = R"RAW(
#version 300 es


layout(location = 0) in highp vec3 position;
layout(location = 1) in highp vec4 color;
layout(location = 2) in highp vec3 uv;

uniform mat4 mvp;
uniform mat4 mv;

out highp vec4 v_color;
out highp vec3 v_uv;
out highp vec4 v_view_pos;
out highp vec4 v_proj_pos;

void main()
{
  gl_Position =  vec4(position, 1.0) * mvp;

  v_color = color;
  v_uv = uv;
  v_view_pos = vec4(position, 1.0) * mv;
  v_proj_pos = gl_Position;
}

)RAW";

static const char* frag_shader = R"RAW(
#version 300 es

uniform sampler2D tex0;

in highp vec4 v_color;
in highp vec3 v_uv;
in highp vec4 v_view_pos;
in highp vec4 v_proj_pos;

out highp vec4 color;

void main()
{
    highp float red = texture(tex0, v_uv.xy ).r;
    color = vec4(red) + vec4(0.5);

}

)RAW";

static void setup()
{
    gfx::mat4x4_identity(model0);
    gfx::mat4x4_identity(projection);
    gfx::mat4x4_identity(view);

    gfx::mat4x4_translate(model0, -0.5, -0.5, 0);

    gfx::mat4x4 tmp;
    gfx::mat4x4_identity(tmp);
    gfx::mat4x4_scale_aniso(tmp, tmp, 100, 100, 100);

    gfx::mat4x4_mul(model0, model0, tmp);

    gfx::mat4x4_dump(model0);

    // clang-format off

    float eye[] = {0, 0, 100};
    float center[] = {0, 0, 0};
    float up[] = {0, 1, 0};

    gfx::mat4x4_look_at(view, eye, center, up);

    gfx::mat4x4_perspective(projection, PI*0.5, 1, 0.05, 1000);
    // gfx::mat4x4_ortho(projection, -1, 1, -1, 1, 0, 14);

    static float vertices[][10] = {
        { 0, 0, 0, 1, 0, 1, 1 ,0,0,0,},
        { 1, 0, 0, 0, 1, 1, 1 ,1,0,0,},
        { 1, 1, 0, 1, 1, 1, 1 ,1,1,0,},
        { 0, 1, 0, 1, 1, 0, 1 ,0,1,0,},
    };

    static uint32_t indices[] = { 
        0, 2, 1, 0, 3, 2 ,
    };
    // clang-format on
    gfx::set_clear_color(gfx::rgba(0, 0, 0, 255));
    vbo = gfx::create_vertex_buffer(vertex_layout, 4, vertices);
    ibo = gfx::create_indice_buffer(6, indices);

    program = gfx::create_program(vert_shader, frag_shader);

    gfx::set_projection_transform(projection);
    gfx::set_view_transform(view);

    font = gfx::create_builtin_font();
}

static void cleanup()
{
    gfx::destroy(font);
    gfx::destroy(vbo);
    gfx::destroy(ibo);
    gfx::destroy(program);
}

static void update(double dt)
{
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
    gfx::set_texture(0, gfx::font_texture(font, 0));
    gfx::set_blend_func(gfx::blend::one, gfx::blend::zero);
    gfx::draw_element(0, 0, 6);

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
    cfg.title = "font";
    cfg.options = gfx::MSAA;
    cfg.width = 800;
    cfg.height = 800;
    cfg.setup = setup;
    cfg.cleanup = cleanup;
    cfg.update = update;
    cfg.key_callback = key_callback;
    cfg.mouse_pos_callback = mouse_pos_callback;

    gfx::run(cfg);
    return 0;
}