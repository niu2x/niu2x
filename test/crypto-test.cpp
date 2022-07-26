#include "gtest/gtest.h"
#include "mu-wan-qing.h"
#include <niu2x/crypto.h>
#include <niu2x/type.h>
#include <niu2x/build.h>
#if defined(OpenSSL_FOUND)

struct test_t {
    struct nx::list_t list;
};

TEST(crypto, digest)
{
    std::string target = "f8b4177e10efeb703d6117a72f71dc5c";
    std::string result = nx::crypto::md5(muwanqing, muwanqing_length);
    result = nx::crypto::hex(result);
    EXPECT_EQ(result, target);
}

TEST(crypto, hex)
{
    std::string target = "";
    std::string result = nx::crypto::hex("");
    EXPECT_EQ(result, target);
}

static const char key[]
    = "\x6F\xAE\x3B\x90\x6C\xF2\xE6\x54\x33\x2C\xD2\x3F\xF9\x52\xBA\x28";
static const char iv[]
    = "\x11\x22\x33\x44\x55\x66\x77\x88\x99\x00\x11\x22\x33\x44\x55\x66";

TEST(crypto, cipher)
{
    std::string target = "c00ec6f6c893098aa6f45f4578f94f94";

    auto result = nx::crypto::aes_128_cbc(
        nx::crypto::encrypt, muwanqing, muwanqing_length, key, iv);
    auto cipher = result;

    result = nx::crypto::md5(result.c_str(), result.size());
    result = nx::crypto::hex(result);

    EXPECT_EQ(result, target);

    result = nx::crypto::aes_128_cbc(nx::crypto::decrypt, cipher, key, iv);
    EXPECT_EQ(result, std::string(muwanqing, muwanqing + muwanqing_length));
}

#endif