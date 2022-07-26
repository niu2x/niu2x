#include <niu2x/bitmap.h>

#define BITMAP_FIRST_WORD_MASK(start)                                          \
    (~NX_U64(0) << ((start) & (NX_BITS_PER_U64 - 1)))
#define BITMAP_LAST_WORD_MASK(nbits)                                           \
    (~NX_U64(0) >> (-(nbits) & (NX_BITS_PER_U64 - 1)))

namespace nx {

uint64_t* bitmap_alloc(int nbits)
{
    return (uint64_t*)malloc(NX_BITS_TO_U64(nbits) * NX_BITS_PER_U64);
}

uint64_t* bitmap_zalloc(int nbits)
{
    return (uint64_t*)calloc(NX_BITS_TO_U64(nbits), NX_BITS_PER_U64);
}

void bitmap_free(const uint64_t* self) { free((void*)self); }

int bitmap_and(
    uint64_t* dst, const uint64_t* bitmap1, const uint64_t* bitmap2, int bits)
{
    unsigned int k;
    unsigned int lim = bits / NX_BITS_PER_U64;
    uint64_t result = 0;

    for (k = 0; k < lim; k++)
        result |= (dst[k] = bitmap1[k] & bitmap2[k]);
    if (bits % NX_BITS_PER_U64)
        result
            |= (dst[k] = bitmap1[k] & bitmap2[k] & BITMAP_LAST_WORD_MASK(bits));
    return result != 0;
}

void bitmap_or(
    uint64_t* dst, const uint64_t* bitmap1, const uint64_t* bitmap2, int bits)
{
    unsigned int k;
    unsigned int nr = NX_BITS_TO_U64(bits);

    for (k = 0; k < nr; k++)
        dst[k] = bitmap1[k] | bitmap2[k];
}

void bitmap_xor(
    uint64_t* dst, const uint64_t* bitmap1, const uint64_t* bitmap2, int bits)
{
    unsigned int k;
    unsigned int nr = NX_BITS_TO_U64(bits);

    for (k = 0; k < nr; k++)
        dst[k] = bitmap1[k] ^ bitmap2[k];
}

} // namespace nx