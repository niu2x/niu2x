#include <atomic>
#include <set>
#include <mutex>
#include <niu2x/bitmap.h>
#include <niu2x/errcode.h>
#include "gtest/gtest.h"

TEST(bitmap, bitmap)
{

    auto bitmap = nx::bitmap_zalloc(1024);
    for (int i = 0; i < 1024; i++) {
        EXPECT_EQ(0, nx::bitmap_get(bitmap, i));
        EXPECT_EQ(i, nx::bitmap_find_next_zero(bitmap, 1024, i));
    }

    for (int i = 0; i < 1024; i++) {
        EXPECT_EQ(0, nx::bitmap_get_value8(bitmap, i));
    }

    for (int i = 0; i < 1024; i += 8) {
        nx::bitmap_set(bitmap, i);
    }

    for (int i = 0; i < 1024; i++) {
        EXPECT_EQ(i % 8 == 0 ? 1 : 0, 1 & nx::bitmap_get_value8(bitmap, i));
        EXPECT_EQ(
            i % 8 == 0 ? i + 1 : i, nx::bitmap_find_next_zero(bitmap, 1024, i));
    }

    for (int i = 0; i < 1024; i++) {
        EXPECT_EQ(i % 8 == 0 ? 1 : 0, nx::bitmap_get(bitmap, i));
    }

    nx::bitmap_set(bitmap, 1, 512);

    for (int i = 0; i < 1024; i++) {
        int ll = (((i % 8 == 0) || i < 512) ? (std::max(513, (1 + i))) : i);
        int qq = nx::bitmap_find_next_zero(bitmap, 1024, i);
        EXPECT_EQ(ll, qq);
    }

    nx::bitmap_free(bitmap);
}
