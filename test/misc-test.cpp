#include "gtest/gtest.h"
#include <niu2x/uuid.h>
#include <niu2x/ref.h>
#include <thread>

TEST(misc, uuid)
{
    auto uuid = nx::uuid_str_gen();
    EXPECT_EQ(uuid.size(), 36);

    for (int i = 0; i < 1024; i++) {
        EXPECT_NE(uuid, nx::uuid_str_gen());
    }
}

struct foo_t {
    nx::ref_t ref;
    void* data;
};

static std::atomic<int> foo_counter { 0 };

foo_t* foo_alloc()
{
    foo_t* foo = (foo_t*)malloc(sizeof(struct foo_t));
    nx::ref_init(&(foo->ref));
    foo->data = malloc(1 << 20);
    foo_counter++;
    return foo;
}

void foo_free(foo_t* foo)
{
    free(foo->data);
    free(foo);
    foo_counter--;
}

foo_t* foo_get(foo_t* foo)
{
    nx::ref_get(&(foo->ref));
    return foo;
}

void foo_release(nx::ref_t* ref)
{
    foo_t* foo = NX_CONTAINER_OF(ref, foo_t, ref);
    foo_free(foo);
}

void foo_put(foo_t* foo) { nx::ref_put(&(foo->ref), foo_release); }

TEST(misc, ref)
{

    auto foo = foo_alloc();

    std::thread th11([foo]() {
        for (int i = 0; i < 1024; i++)
            foo_get(foo);
    });

    std::thread th12([foo]() {
        for (int i = 0; i < 1024; i++)
            foo_get(foo);
    });

    std::thread th13([foo]() {
        for (int i = 0; i < 1024; i++)
            foo_get(foo);
    });

    th11.join();
    th12.join();
    th13.join();

    std::thread th21([foo]() {
        for (int i = 0; i < 1025; i++)
            foo_put(foo);
    });

    std::thread th22([foo]() {
        for (int i = 0; i < 1024; i++)
            foo_put(foo);
    });

    std::thread th23([foo]() {
        for (int i = 0; i < 1024; i++)
            foo_put(foo);
    });

    th21.join();
    th22.join();
    th23.join();

    EXPECT_EQ(0, foo_counter);
}
