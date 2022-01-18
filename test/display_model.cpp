#include <niu2x/gfh.h>
#include <niu2x/log.h>
#include <niu2x/gfx_utils.h>

using namespace nx;

static const char* ambient_vert = R"RAW(
#version 300 es
precision highp float;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 uv;

out vec3 v_uv;

uniform mat4 MVP;

void main()
{
  gl_Position =  vec4(position, 1.0) * MVP;
  v_uv = uv;
}

)RAW";

static const char* ambient_frag = R"RAW(
#version 300 es
#define PI 3.14159
precision highp float;
#define screenWidth 1.0

uniform float TIME;

out vec4 color;
in vec3 v_uv;

float rand(vec2 c){
    return fract(sin(dot(c.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float noise(vec2 p, float freq ){
    float unit = screenWidth/freq;
    vec2 ij = floor(p/unit);
    vec2 xy = mod(p,unit)/unit;
    //xy = 3.*xy*xy-2.*xy*xy*xy;
    xy = .5*(1.-cos(PI*xy));
    float a = rand((ij+vec2(0.,0.)));
    float b = rand((ij+vec2(1.,0.)));
    float c = rand((ij+vec2(0.,1.)));
    float d = rand((ij+vec2(1.,1.)));
    float x1 = mix(a, b, xy.x);
    float x2 = mix(c, d, xy.x);
    return mix(x1, x2, xy.y);
}

float pNoise(vec2 p, int res){
    float persistance = .5;
    float n = 0.;
    float normK = 0.;
    float f = 4.;
    float amp = 1.;
    int iCount = 0;
    for (int i = 0; i<50; i++){
        n+=amp*noise(p, f);
        f*=2.;
        normK+=amp;
        amp*=persistance;
        if (iCount == res) break;
        iCount++;
    }
    float nf = n/normK;
    return nf*nf*nf*nf;
}

void main()
{
    //color = vec4(pNoise(v_uv.xy+vec2(sin(TIME*0.2)), 1), pNoise(v_uv.xy+vec2(cos(TIME*0.2)), 2), pNoise(v_uv.xy+vec2(TIME*0.2), 3), 1.0);
    color = vec4(1.0);       
}

)RAW";

class my_app_t : public gfh::app_t {
public:
    virtual void setup()
    {
        floor_ = gfx_utils::mesh_create_plane(1024, 1024);
        ambient_ = gfx::program_create(ambient_vert, ambient_frag);

        math::vec3 eye { 100, 100, 100 };
        math::vec3 center { 0, 0, 0 };
        math::vec3 up { 0, 0, 1 };
        camera_.look_at(eye, center, up);

        right_button_pressing_ = false;

        object_ = gfx::mesh_group_create_from_file("../resource/cubes.fbx", 0);
    }

    virtual void cleanup()
    {
        gfx::destroy(floor_);
        gfx::destroy(ambient_);
        gfx::destroy(object_);
    }

    virtual void update(double dt)
    {
        gfx::begin();
        gfx::set_render_state(gfx::WRITE_RGBA | gfx::DEPTH_TEST
            | gfx::WRITE_DEPTH | gfx::PLOYGON_MODE_LINE);
        gfx::clear(0);

        math::mat4x4 projection;
        math::mat4x4 model;
        auto& view = camera_.transform();

        math::mat4x4_perspective(projection, math::pi * 0.5, 1.2, 0.1, 1000000);
        math::mat4x4_identity(model);

        gfx::set_model_transform(model);
        gfx::set_view_transform(view);
        gfx::set_projection_transform(projection);
        gfx::set_program(ambient_);
        // gfx::draw_mesh(0, floor_);

        math::mat4x4_scale_aniso(model, 30, 30, 30);
        gfx::set_model_transform(model);
        gfx::draw_mesh_group(0, object_);

        gfx::end();
    }

    virtual std::string title() const { return "display model"; }

    virtual void input_mouse_move(double x, double y)
    {
        static double last_x = x;
        static double last_y = y;

        if (right_button_pressing_) {
            camera_.pitch((y - last_y) / math::pi / 100.0);
            camera_.yaw((x - last_x) / math::pi / 100.0);
        }
        last_x = x;
        last_y = y;
    }

    virtual void input_key(int keycode, int action, int mods)
    {
        right_button_pressing_ = keycode == gfx::MOUSE_RIGHT_BUTTON
            && (action == gfx::KEY_PRESS || action == gfx::KEY_REPEAT);
    }

private:
    gfx::mesh_t* floor_;
    gfx::program_t* ambient_;
    gfh::camera_t camera_;
    gfx::mesh_group_t* object_;
    bool right_button_pressing_;
};

NX_GFH_ENTRY(my_app_t);