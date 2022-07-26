#include "gtest/gtest.h"
#include <niu2x/uuid.h>

TEST(misc, uuid)
{
    auto uuid = nx::uuid_str_gen();
    EXPECT_EQ(uuid.size(), 36);

    for (int i = 0; i < 1024; i++) {
        // EXPECT_NE(uuid, nx::uuid_str_gen());
    }
}
