#include "gfh.h"

namespace nx::gfh {

game_app_t::game_app_t() { }
game_app_t::~game_app_t() { }

void game_app_t::setup()
{
    gfh::setup();
    world_setup(&world);
}
void game_app_t::cleanup()
{

    world_cleanup(&world);

    gfh::cleanup();
}
void game_app_t::update(double dt) { unused(dt); }

} // namespace nx::gfh