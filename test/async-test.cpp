#include <atomic>
#include <set>
#include <mutex>
#include <niu2x/async.h>
#include "gtest/gtest.h"

TEST(async, thread_group)
{
    std::atomic<int> k { 0 };

    using tid_t = std::decay_t<decltype(std::this_thread::get_id())>;

    std::set<tid_t> tids;
    std::mutex mutex;

    nx::async::thread_group_t ths;
    for (int i = 0; i < 1024; i++) {
        ths.add_thread([&mutex, &k, &tids]() {
            for (int i = 0; i < 1024; i++)
                ++k;
            {
                std::lock_guard<std::mutex> _(mutex);
                tids.insert(std::this_thread::get_id());
            }
        });
    }
    ths.join_all();

    EXPECT_EQ(1 << 20, k);
}

TEST(async, io_context)
{
    std::atomic<int> k { 0 };

    using tid_t = std::decay_t<decltype(std::this_thread::get_id())>;

    std::set<tid_t> tids;
    std::mutex mutex;

    nx::async::thread_group_t ths;
    nx::async::io_context_t context;

    for (int i = 0; i < 1024 * 1024; i++) {
        context.post([&k, &tids, &mutex](void*) {
            for (int i = 0; i < 4; i++)
                ++k;
            {
                std::lock_guard<std::mutex> _(mutex);
                tids.insert(std::this_thread::get_id());
            }
        });
    }

    for (int i = 0; i < 2; i++) {
        ths.add_thread([&context]() { context.run(nullptr); });
    }

    ths.join_all();

    EXPECT_EQ(4 * (1 << 20), k);
}