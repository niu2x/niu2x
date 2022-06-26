#include "gtest/gtest.h"
#include <niu2x/algorithm.h>

int int_compare(const void* a, const void* b)
{
    const int* aa = (const int*)(a);
    const int* bb = (const int*)(b);
    return *aa - *bb;
}

TEST(algorithm, binary_search1)
{
    int array[] = { 1, 3, 5, 7, 9, 11, 13, 15, 17, 19 };
    int nr = sizeof(array) / sizeof(int);
    int target = 7;
    int result
        = nx::algo::binary_search(array, nr, sizeof(int), &target, int_compare);
    EXPECT_EQ(result, 3);

    target = 1;
    result
        = nx::algo::binary_search(array, nr, sizeof(int), &target, int_compare);
    EXPECT_EQ(result, 0);

    target = 19;
    result
        = nx::algo::binary_search(array, nr, sizeof(int), &target, int_compare);
    EXPECT_EQ(result, 9);

    target = 0;
    result
        = nx::algo::binary_search(array, nr, sizeof(int), &target, int_compare);
    EXPECT_EQ(result, -1);

    target = 2;
    result
        = nx::algo::binary_search(array, nr, sizeof(int), &target, int_compare);
    EXPECT_EQ(result, -2);

    target = 1024;
    result
        = nx::algo::binary_search(array, nr, sizeof(int), &target, int_compare);
    EXPECT_EQ(result, -11);
}

TEST(algorithm, binary_search2)
{
    int array[] = { 1, 3, 5, 7, 9, 11, 13, 15, 17, 19 };
    int nr = sizeof(array) / sizeof(int);

    int target = 7;
    int result = nx::algo::binary_search(array, nr, &target);
    EXPECT_EQ(result, 3);

    target = 1;
    result = nx::algo::binary_search(array, nr, &target);
    EXPECT_EQ(result, 0);

    target = 19;
    result = nx::algo::binary_search(array, nr, &target);
    EXPECT_EQ(result, 9);

    target = 0;
    result = nx::algo::binary_search(array, nr, &target);
    EXPECT_EQ(result, -1);

    target = 2;
    result = nx::algo::binary_search(array, nr, &target);
    EXPECT_EQ(result, -2);

    target = 1024;
    result = nx::algo::binary_search(array, nr, &target);
    EXPECT_EQ(result, -11);
}

struct stu_t {
    int age;

    bool operator==(const stu_t& other) const { return age == other.age; }

    bool operator<(const stu_t& other) const { return age < other.age; }
};

TEST(algorithm, binary_search3)
{
    stu_t array[] = { { 1 }, { 3 }, { 5 }, { 7 }, { 9 }, { 11 }, { 13 }, { 15 },
        { 17 }, { 19 } };
    int nr = sizeof(array) / sizeof(int);

    stu_t target = { 7 };
    int result = nx::algo::binary_search(array, nr, &target);
    EXPECT_EQ(result, 3);

    target.age = 1;
    result = nx::algo::binary_search(array, nr, &target);
    EXPECT_EQ(result, 0);

    target.age = 19;
    result = nx::algo::binary_search(array, nr, &target);
    EXPECT_EQ(result, 9);

    target.age = 0;
    result = nx::algo::binary_search(array, nr, &target);
    EXPECT_EQ(result, -1);

    target.age = 2;
    result = nx::algo::binary_search(array, nr, &target);
    EXPECT_EQ(result, -2);

    target.age = 1024;
    result = nx::algo::binary_search(array, nr, &target);
    EXPECT_EQ(result, -11);
}
