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

        gfx::draw_texture(0, tex);

        gfx::end();
    }

    virtual std::string title() const override { return "empty window"; }

    virtual nx::math::vec2_t window_size() const override
    {
        return nx::math::vec2_t { 512, 512 };
    }

    virtual void setup() override
    {
        tex = gfx::texture_create_from_file("../test/floor-stone.jpg");
    }

    virtual void cleanup() override { gfx::destroy(tex); }

private:
    gfx::texture_t* tex;
};

NX_GFH_ENTRY(my_app_t);