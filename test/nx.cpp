#include <niu2x/gfh.h>
#include <niu2x/log.h>
#include <niu2x/gfx_utils.h>

using namespace nx;

class my_app_t : public gfh::app_t {
public:
    virtual void setup() { }

    virtual void cleanup() { }

    virtual void update(double dt) { }

    virtual std::string title() const { return "display model"; }

    virtual void input_mouse_move(double x, double y) { }

    virtual void input_key(int keycode, int action, int mods) { }

private:
};

NX_GFH_ENTRY(my_app_t);