#ifndef NX_GFH_H
#define NX_GFH_H

#include <boost/preprocessor.hpp>

#include <niu2x/gfx.h>
#include <niu2x/linmath.h>
#include <niu2x/hashtab.h>
#include <niu2x/object.h>

namespace nx::gfh {

namespace gfx = nx::gfx;
namespace math = nx::math;

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

struct NXAPI component_t : nx::object_t {
    enum {
        transform,
    };
    component_type_t type;
    hashtab_entry_t hash;
    uint64_t ref;
};

struct NXAPI transform_t : component_t {
    math::vec3 position;
    math::vec3 rotation;
    math::vec3 scale;
};

struct NXAPI game_object_t : nx::object_t {
    hashtab_t components;
    list_t children;

    game_object_t* parent;
    list_t self_in_children; // valid if only parent is't nullptr
    uint64_t ref;
};

struct NXAPI world_t : nx::object_t {
    game_object_t* root;
};

NXAPI void world_setup(world_t* self);
NXAPI void world_cleanup(world_t* self);
NXAPI void world_update(world_t* self, double dt);

NXAPI void game_object_add_child(game_object_t* self, game_object_t* child);
NXAPI void game_object_remove_children(game_object_t* self);

NXAPI game_object_t* game_object_create();
NXAPI inline void game_object_retain(game_object_t* self) { self->ref++; }
NXAPI void game_object_release(game_object_t* self);

NXAPI transform_t* transform_create();
NXAPI void transform_retain(transform_t *);
NXAPI void transform_release(transform_t *);

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

private:
};

class NXAPI game_app_t : public app_t {
public:
    game_app_t();
    virtual ~game_app_t();

    virtual void setup() override;
    virtual void cleanup() override;
    virtual void update(double dt) override;
    virtual std::string title() const override { return ""; }
    virtual uint64_t window_options() const override { return gfx::MSAA; }
    virtual math::vec2_t window_size() const override { return { 800, 600 }; };
    virtual void input_mouse_move(double xpos, double ypos) override
    {
        (void)(xpos), (void)ypos;
    }
    virtual void input_key(int keycode, int action, int mods) override
    {
        (void)keycode;
        (void)action;
        (void)mods;
    }

private:
    world_t world_;
};

NXAPI void setup();
NXAPI void cleanup();

#define NX_GFH_ENTRY(clazz)                                                    \
                                                                               \
    namespace {                                                                \
                                                                               \
    static nx::gfh::app_t* app;                                                \
                                                                               \
    static void update(double dt) { app->update(dt); }                         \
    static void setup()                                                        \
    {                                                                          \
        nx::gfh::setup();                                                      \
        app->setup();                                                          \
    }                                                                          \
    static void cleanup()                                                      \
    {                                                                          \
        app->cleanup();                                                        \
        nx::gfh::cleanup();                                                    \
    }                                                                          \
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

} // namespace nx::gfh

#endif
