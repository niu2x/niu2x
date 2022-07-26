#ifndef NX_BITMAP_H
#define NX_BITMAP_H

#include <string.h>

#include <niu2x/api.h>
#include <niu2x/utils.h>

namespace nx {

NXAPI uint64_t* bitmap_alloc(int nbits);
NXAPI uint64_t* bitmap_zalloc(int nbits);
NXAPI void bitmap_free(const uint64_t* bitmap);

inline void bitmap_zero(uint64_t* dst, int nbits)
{
    auto len = NX_BITS_TO_U64(nbits) * sizeof(uint64_t);
    memset(dst, 0, len);
}

inline void bitmap_fill(uint64_t* dst, int nbits)
{
    unsigned int len = NX_BITS_TO_U64(nbits) * sizeof(uint64_t);
    memset(dst, 0xff, len);
}

inline void bitmap_copy(uint64_t* dst, const uint64_t* src, int nbits)
{
    unsigned int len = NX_BITS_TO_U64(nbits) * sizeof(uint64_t);
    memcpy(dst, src, len);
}

NXAPI int bitmap_and(
    uint64_t* dst, const uint64_t* src1, const uint64_t* src2, int nbits);
NXAPI void bitmap_or(
    uint64_t* dst, const uint64_t* src1, const uint64_t* src2, int nbits);
NXAPI void bitmap_xor(
    uint64_t* dst, const uint64_t* src1, const uint64_t* src2, int nbits);
NXAPI int bitmap_set(uint64_t* map, int start, int nbits);
NXAPI int bitmap_clear(uint64_t* map, int start, int nbits);

} // namespace nx

#endif