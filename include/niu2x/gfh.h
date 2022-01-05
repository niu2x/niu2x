#ifndef NX_GFH_H
#define NX_GFH_H

#include <boost/preprocessor.hpp>

#include <niu2x/gfx.h>
#include <niu2x/linmath.h>

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
    static void key_callback(int keycode, int action, int mods) { }            \
    static void mouse_pos_callback(double xpos, double ypos) { }               \
    }                                                                          \
                                                                               \
    int main()                                                                 \
    {                                                                          \
        clazz my_app;                                                          \
        app = &my_app;                                                         \
        nx::gfx::window_config config {                                        \
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