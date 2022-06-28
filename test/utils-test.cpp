#include "gtest/gtest.h"
#include <niu2x/utils.h>

TEST(utils, container_of)
{

    struct stu_t {
        int k1;
        int k2;
    };

    EXPECT_EQ(0, NX_OFFSET_OF(struct stu_t, k1));
    EXPECT_EQ(sizeof(int), NX_OFFSET_OF(struct stu_t, k2));

    stu_t st;
    EXPECT_EQ(&st, NX_CONTAINER_OF(&st.k2, stu_t, k2));
    EXPECT_NE(&st, NX_CONTAINER_OF(&st.k2, stu_t, k1));
    EXPECT_NE(&st, NX_CONTAINER_OF(&st.k1, stu_t, k2));
    EXPECT_EQ(&st, NX_CONTAINER_OF(&st.k1, stu_t, k1));
}