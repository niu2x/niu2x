#include "gtest/gtest.h"
#include <niu2x/async/event_loop.h>
#include <niu2x/build.h>

#if defined(libuv_FOUND)
TEST(async, event_loop) { nx::async::event_loop_t event_loop; }
#endif