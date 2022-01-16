#include <niu2x/gfh.h>

using namespace nx;

class my_app_t : public gfh::app_t {
public:
    virtual void setup() { }
    virtual void cleanup() { }
    virtual void update(double dt)
    {
        gfx::begin();
        gfx::set_render_state(gfx::WRITE_RGBA);
        gfx::clear(0);
        gfx::end();
    }

    virtual std::string title() const { return "display model"; }
};

NX_GFH_ENTRY(my_app_t);