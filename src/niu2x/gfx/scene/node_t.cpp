#include "node_t.h"

namespace nx::gfx::scene {

node_t::id_t node_t::next_id_ = 1;

node_t::node_t()
: id_(next_id_++)
{
}

node_t::~node_t() { }

} // namespace nx::gfx::scene