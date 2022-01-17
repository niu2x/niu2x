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
    }

    virtual void cleanup()
    {
        gfx::destroy(floor_);
        gfx::destroy(ambient_);
    }

    virtual void update(double dt)
    {
        gfx::begin();
        gfx::set_render_state(
            gfx::WRITE_RGBA | gfx::DEPTH_TEST | gfx::WRITE_DEPTH);
        gfx::clear(0);

        math::mat4x4 projection;
        math::mat4x4 model;
        auto& view = camera_.transform();

        math::mat4x4_perspective(projection, math::pi * 0.5, 1.2, 0.1, 1000);
        math::mat4x4_identity(model);

        gfx::set_model_transform(model);
        gfx::set_view_transform(view);
        gfx::set_projection_transform(projection);
        gfx::set_program(ambient_);
        gfx::draw_mesh(0, floor_);

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
        // if (action == gfx::KEY_PRESS || action == gfx::KEY_REPEAT) {
        //     if (keycode == gfx::KEY_W) {
        //         camera_.move(10);
        //     }
        //     if (keycode == gfx::KEY_S) {
        //         camera_.move(-10);
        //     }
        // }

        right_button_pressing_ = keycode == gfx::MOUSE_RIGHT_BUTTON
            && (action == gfx::KEY_PRESS || action == gfx::KEY_REPEAT);
    }

private:
    gfx::mesh_t* floor_;
    gfx::program_t* ambient_;
    gfh::camera_t camera_;
    bool right_button_pressing_;
};

NX_GFH_ENTRY(my_app_t);