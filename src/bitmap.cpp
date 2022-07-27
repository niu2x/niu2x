#include <niu2x/bitmap.h>

#define BITMAP_FIRST_WORD_MASK(start)                                          \
    (~NX_U64(0) << ((start) & (NX_BITS_PER_U64 - 1)))
#define BITMAP_LAST_WORD_MASK(nbits)                                           \
    (~NX_U64(0) >> (-(nbits) & (NX_BITS_PER_U64 - 1)))

namespace nx {

bitmap_t bitmap_alloc(int nbits)
{
    return (bitmap_t)malloc(NX_BITS_TO_U64(nbits) * NX_BITS_PER_U64);
}

bitmap_t bitmap_zalloc(int nbits)
{
    return (bitmap_t)calloc(NX_BITS_TO_U64(nbits), NX_BITS_PER_U64);
}

void bitmap_free(const bitmap_t self) { free((void*)self); }

int bitmap_and(
    bitmap_t dst, const bitmap_t bitmap1, const bitmap_t bitmap2, int bits)
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
    bitmap_t dst, const bitmap_t bitmap1, const bitmap_t bitmap2, int bits)
{
    unsigned int k;
    unsigned int nr = NX_BITS_TO_U64(bits);

    for (k = 0; k < nr; k++)
        dst[k] = bitmap1[k] | bitmap2[k];
}

void bitmap_xor(
    bitmap_t dst, const bitmap_t bitmap1, const bitmap_t bitmap2, int bits)
{
    unsigned int k;
    unsigned int nr = NX_BITS_TO_U64(bits);

    for (k = 0; k < nr; k++)
        dst[k] = bitmap1[k] ^ bitmap2[k];
}

void bitmap_set(bitmap_t map, int start, int len)
{
    bitmap_t p = map + NX_BIT_U64_WORD(start);
    const unsigned int size = start + len;
    int bits_to_set = NX_BITS_PER_U64 - (start % NX_BITS_PER_U64);
    uint64_t mask_to_set = BITMAP_FIRST_WORD_MASK(start);

    while (len - bits_to_set >= 0) {
        *p |= mask_to_set;
        len -= bits_to_set;
        bits_to_set = NX_BITS_PER_U64;
        mask_to_set = ~0UL;
        p++;
    }
    if (len) {
        mask_to_set &= BITMAP_LAST_WORD_MASK(size);
        *p |= mask_to_set;
    }
}

void bitmap_clear(bitmap_t map, int start, int len)
{
    bitmap_t p = map + NX_BIT_U64_WORD(start);
    const unsigned int size = start + len;
    int bits_to_clear = NX_BITS_PER_U64 - (start % NX_BITS_PER_U64);
    uint64_t mask_to_clear = BITMAP_FIRST_WORD_MASK(start);

    while (len - bits_to_clear >= 0) {
        *p &= ~mask_to_clear;
        len -= bits_to_clear;
        bits_to_clear = NX_BITS_PER_U64;
        mask_to_clear = ~0UL;
        p++;
    }
    if (len) {
        mask_to_clear &= BITMAP_LAST_WORD_MASK(size);
        *p &= ~mask_to_clear;
    }
}

int bitmap_find_next_zero(bitmap_t map, int nbits, int start)
{
    if (start >= nbits)
        return -1;

    size_t index = NX_BIT_U64_WORD(start);
    int n = start % NX_BITS_PER_U64;

    if (~((map[index]) | (NX_BIT_U64_MASK(n) - 1)) != 0) {
        uint64_t k = map[index] >> n;
        int step = 0;
        while (k & 1) {
            k >>= 1;
            step++;
        }

        int result = start + step;
        if (result < nbits) {
            return result;
        } else
            return -1;
    }

    const size_t last_index = NX_BIT_U64_WORD(nbits - 1);
    index++;
    while (index < last_index && ~map[index] == 0)
        index++;

    if (~map[index] == 0)
        return -1;

    uint64_t k = map[index];
    int step = 0;
    while (k & 1) {
        k >>= 1;
        step++;
    }

    int result = index * NX_BITS_PER_U64 + step;
    if (result < nbits) {
        return result;
    } else
        return -1;
}

// int bitmap_find_next_zero_area_off(bitmap_t map, int size, int start, int nr,
//     uint64_t align_mask, uint64_t align_offset)
// {
//     unsigned long index, end, i;
// again:
//     index = find_next_zero_bit(map, size, start);

//     /* Align allocation */
//     index = __ALIGN_MASK(index + align_offset, align_mask) - align_offset;

//     end = index + nr;
//     if (end > size)
//         return end;
//     i = find_next_bit(map, end, index);
//     if (i < end) {
//         start = i + 1;
//         goto again;
//     }
//     return index;
// }

} // namespace nx