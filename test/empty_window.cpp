#include <niu2x/gfh.h>
#include <niu2x/log.h>

#include <math.h>

namespace gfx = nx::gfx;

class my_app_t : public nx::gfh::app_t {
public:
    my_app_t() { }
    virtual ~my_app_t() { }
    virtual void update(double dt) override
    {
        gfx::begin();
        gfx::set_render_state(gfx::WRITE_RGBA);
        gfx::clear(0);
        gfx::end();
    }

    virtual std::string title() const override { return "empty window"; }

    virtual nx::math::vec2_t window_size() const override
    {
        return nx::math::vec2_t { 512, 512 };
    }
};

NX_GFH_ENTRY(my_app_t);