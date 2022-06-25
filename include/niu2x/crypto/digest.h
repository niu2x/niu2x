#ifndef NX_CRYPTO_DIGEST_H
#define NX_CRYPTO_DIGEST_H

#include <string>
#include <boost/preprocessor.hpp>
#include <niu2x/utils.h>
#include <niu2x/api.h>
#include <niu2x/errcode.h>

// clang-format off
#define NX_CRYPTO_DIGEST_ALGORITHMS()   \
                        (md5)           \
                        (sha256)        \
                        (blake2b512)    \
                        (blake2s256)    \
                        (md5_sha1)      \
                        (sha1)          \
                        (sha224)        \
                        (sha3_224)      \
                        (sha3_256)      \
                        (sha3_384)      \
                        (sha3_512)      \
                        (sha384)        \
                        (sha512)        \
                        (sha512_224)    \
                        (sha512_256)    \
                        (shake128)      \
                        (shake256)      \
                        (sm3)           \
                        (ssl3_md5)      \
                        (ssl3_sha1)

// clang-format on

namespace nx::crypto {

#define DECLARE_DIGEST(r, data, alo)                                           \
    NXAPI int BOOST_PP_CAT(alo, _size)(const void* input, int size);           \
    NXAPI int alo(void* out, const void* input, int size);                     \
    NXAPI std::string alo(const void* input, int size);                        \
    NXAPI std::string alo(const std::string&);

BOOST_PP_SEQ_FOR_EACH(DECLARE_DIGEST, ~, NX_CRYPTO_DIGEST_ALGORITHMS())

#undef DECLARE_DIGEST

} // namespace nx::crypto

#endif