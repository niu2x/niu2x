#include "aio.h"

namespace nx::lua_bindings::aio {

event_loop::event_loop(std::unique_ptr<nx::aio::event_loop>&& delegate)
: delegate_(std::move(delegate))
{
}

event_loop::~event_loop() { }

} // namespace nx::lua_bindings::aio