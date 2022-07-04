#include <niu2x/crypto/hex.h>

#include "common.h"

namespace nx::crypto {

int hex_size(const void* input, int size)
{
    NX_UNUSED(input);
    return size * 2;
}

int hex(void* out, const void* input, int size)
{
    const uint8_t* p = (const uint8_t*)input;
    const uint8_t* end = p + size;
    char* o = (char*)out;

    char buf[4];
    while (p < end) {
        sprintf(buf, "%02x", *p++);
        *(o++) = buf[0];
        *(o++) = buf[1];
    }
    return size * 2;
}

DEFINE_CRYPTO_STR_FUNC(hex)
DEFINE_CRYPTO_STR_FUNC(unhex)
DEFINE_CRYPTO_STR2_FUNC(hex)
DEFINE_CRYPTO_STR2_FUNC(unhex)

int unhex_size(const void* input, int size)
{
    NX_UNUSED(input);
    return size >> 1;
}

static int unhex_char(char c)
{
    if (c <= '9')
        return c - '0';
    if (c <= 'F')
        return 10 + c - 'A';
    if (c <= 'f')
        return 10 + c - 'a';
    // TODO: error log
    return -1;
}

int unhex(void* out, const void* input, int size)
{
    const char* p = (const char*)input;
    const char* end = p + size;
    uint8_t* o = (uint8_t*)out;

    while (p < end) {
        uint8_t n = unhex_char(*p++);
        n = (n << 4) | unhex_char(*p++);
        *o++ = n;
    }

    return size >> 1;
}

} // namespace nx::crypto
