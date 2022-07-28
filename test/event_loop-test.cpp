#include "gtest/gtest.h"
#include <niu2x/async/event_loop.h>
#include <niu2x/build.h>

#if defined(libuv_FOUND)
TEST(async, event_loop)
{

    int data[32] = { 0 };

    nx::async::event_loop_t event_loop;

    for (int i = 0; i < 32; i++)
        event_loop.idle_start([&event_loop, &data](auto id) {
            static int c[32] = { 0 };
            while (c[id]++ < 1024) {
                data[id]++;
            }
            event_loop.idle_stop(id);
            event_loop.idle_start([&event_loop, &data](auto id) {
                data[id] += id;
                event_loop.idle_stop(id);
            });
        });
    event_loop.run();

    for (int i = 0; i < 32; i++)
        EXPECT_EQ(1024 + i, data[i]);
}
#endif