#ifndef NX_GFH_H
#define NX_GFH_H

#include <boost/preprocessor.hpp>

#include <niu2x/gfx.h>
#include <niu2x/linmath.h>
#include <niu2x/hashtab.h>

namespace nx::gfh {

namespace gfx = nx::gfx;
namespace math = nx::math;

class NXAPI app_t : boost::noncopyable {
public:
    app_t();
    virtual ~app_t();

    virtual void setup() { }
    virtual void cleanup() { }
    virtual void update(double dt) { (void)dt; }
    virtual std::string title() const { return ""; }
    virtual uint64_t window_options() const { return gfx::MSAA; }
    virtual math::vec2_t window_size() const { return { 800, 600 }; };
    virtual void input_mouse_move(double xpos, double ypos)
    {
        (void)(xpos), (void)ypos;
    }
    virtual void input_key(int keycode, int action, int mods)
    {
        (void)keycode;
        (void)action;
        (void)mods;
    }
    void run();
};

#define NX_GFH_ENTRY(clazz)                                                    \
                                                                               \
    namespace {                                                                \
                                                                               \
    static nx::gfh::app_t* app;                                                \
                                                                               \
    static void update(double dt) { app->update(dt); }                         \
    static void setup() { app->setup(); }                                      \
    static void cleanup() { app->cleanup(); }                                  \
    static void key_callback(int keycode, int action, int mods)                \
    {                                                                          \
        app->input_key(keycode, action, mods);                                 \
    }                                                                          \
    static void mouse_pos_callback(double xpos, double ypos)                   \
    {                                                                          \
        app->input_mouse_move(xpos, ypos);                                     \
    }                                                                          \
    }                                                                          \
                                                                               \
    int main()                                                                 \
    {                                                                          \
        clazz my_app;                                                          \
        app = &my_app;                                                         \
        nx::gfx::window_config_t config {                                      \
            .title = app->title(),                                             \
            .width = (int)(app->window_size().width),                          \
            .height = (int)(app->window_size().height),                        \
            .options = app->window_options(),                                  \
            .update = update,                                                  \
            .setup = setup,                                                    \
            .cleanup = cleanup,                                                \
            .key_callback = key_callback,                                      \
            .mouse_pos_callback = mouse_pos_callback,                          \
        };                                                                     \
        nx::gfx::run(config);                                                  \
        return 0;                                                              \
    }

// https://learnopengl.com/img/getting-started/camera_pitch_yaw_roll.png
class NXAPI camera_t {
public:
    camera_t();
    ~camera_t();
    camera_t(const camera_t&) = default;
    camera_t& operator=(const camera_t&) = default;
    void look_at(
        math::vec3 const eye, math::vec3 const center, math::vec3 const up);

    void pitch(double x);
    void yaw(double x);
    void roll(double x);
    void move(double dx, double dy, double dz);

    const math::mat4x4& transform() const { return transform_; }

private:
    math::mat4x4 transform_;
    math::vec3 eye_;
    math::vec3 up_;
    math::vec3 center_;
};

using component_id_t = uint32_t;
using component_type_t = uint8_t;

struct NXAPI component_t {
    enum {
        transform,
    };
    component_type_t type;
    hashtab_entry_t hash;
};

struct NXAPI transform_t : component_t {
    math::vec3 position;
    math::vec3 rotation;
    math::vec3 scale;
};

struct NXAPI game_object_t {
    hashtab_t component_registry;
};

game_object_t* game_object_create();
void destroy(game_object_t*);

} // namespace nx::gfh

#endif