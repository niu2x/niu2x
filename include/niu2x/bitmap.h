#ifndef NX_BITMAP_H
#define NX_BITMAP_H

#include <string.h>

#include <niu2x/api.h>
#include <niu2x/utils.h>

namespace nx {

using bitmap_t = uint64_t*;

NXAPI bitmap_t bitmap_alloc(int nbits);
NXAPI bitmap_t bitmap_zalloc(int nbits);
NXAPI void bitmap_free(const bitmap_t bitmap);

static inline void bitmap_zero(bitmap_t dst, int nbits)
{
    auto len = NX_BITS_TO_U64(nbits) * sizeof(uint64_t);
    memset(dst, 0, len);
}

static inline void bitmap_fill(bitmap_t dst, int nbits)
{
    unsigned int len = NX_BITS_TO_U64(nbits) * sizeof(uint64_t);
    memset(dst, 0xff, len);
}

static inline void bitmap_copy(bitmap_t dst, const bitmap_t src, int nbits)
{
    unsigned int len = NX_BITS_TO_U64(nbits) * sizeof(uint64_t);
    memcpy(dst, src, len);
}

NXAPI int bitmap_and(
    bitmap_t dst, const bitmap_t src1, const bitmap_t src2, int nbits);
NXAPI void bitmap_or(
    bitmap_t dst, const bitmap_t src1, const bitmap_t src2, int nbits);
NXAPI void bitmap_xor(
    bitmap_t dst, const bitmap_t src1, const bitmap_t src2, int nbits);
NXAPI void bitmap_set(bitmap_t map, int start, int nbits = 1);
NXAPI void bitmap_clear(bitmap_t map, int start, int nbits = 1);

static inline uint8_t bitmap_get_value8(const bitmap_t map, int start)
{
    const size_t index = NX_BIT_U64_WORD(start);
    const unsigned long offset = start % NX_BITS_PER_U64;
    return (map[index] >> offset) & 0xFF;
}

static inline void bitmap_set_value8(bitmap_t map, uint8_t value, int start)
{
    const size_t index = NX_BIT_U64_WORD(start);
    const unsigned long offset = start % NX_BITS_PER_U64;

    map[index] &= ~(0xFFUL << offset);
    map[index] |= value << offset;
}

static inline uint8_t bitmap_get(bitmap_t map, int start)
{
    return 1 & bitmap_get_value8(map, start);
}

NXAPI int bitmap_find_next_zero(bitmap_t map, int nbits, int start);

} // namespace nx

#endif