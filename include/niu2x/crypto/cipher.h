#ifndef NX_CRYPTO_CIPHER_H
#define NX_CRYPTO_CIPHER_H

#include <string>
#include <boost/preprocessor.hpp>
#include <niu2x/utils.h>
#include <niu2x/api.h>

// clang-format off
#define NX_CRYPTO_CIPHER_ALGORITHMS()               \
    (aes_128_cbc)                                   \
    (aes_128_cfb)                                   \
    (aes_128_cfb1)                                  \
    (aes_128_cfb8)                                  \
    (aes_128_ctr)                                   \
    (aes_128_ecb)                                   \
    (aes_128_ofb)                                   \
    (aes_192_cbc)                                   \
    (aes_192_cfb)                                   \
    (aes_192_cfb1)                                  \
    (aes_192_cfb8)                                  \
    (aes_192_ctr)                                   \
    (aes_192_ecb)                                   \
    (aes_192_ofb)                                   \
    (aes_256_cbc)                                   \
    (aes_256_cfb)                                   \
    (aes_256_cfb1)                                  \
    (aes_256_cfb8)                                  \
    (aes_256_ctr)                                   \
    (aes_256_ecb)                                   \
    (aes_256_ofb)                                   \
    (aes128)                                        \
    (aes192)                                        \
    (aes256)                                        \
    (aria_128_cbc)                                  \
    (aria_128_cfb)                                  \
    (aria_128_cfb1)                                 \
    (aria_128_cfb8)                                 \
    (aria_128_ctr)                                  \
    (aria_128_ecb)                                  \
    (aria_128_ofb)                                  \
    (aria_192_cbc)                                  \
    (aria_192_cfb)                                  \
    (aria_192_cfb1)                                 \
    (aria_192_cfb8)                                 \
    (aria_192_ctr)                                  \
    (aria_192_ecb)                                  \
    (aria_192_ofb)                                  \
    (aria_256_cbc)                                  \
    (aria_256_cfb)                                  \
    (aria_256_cfb1)                                 \
    (aria_256_cfb8)                                 \
    (aria_256_ctr)                                  \
    (aria_256_ecb)                                  \
    (aria_256_ofb)                                  \
    (aria128)                                       \
    (aria192)                                       \
    (aria256)                                       \
    (camellia_128_cbc)                              \
    (camellia_128_cfb)                              \
    (camellia_128_cfb1)                             \
    (camellia_128_cfb8)                             \
    (camellia_128_ctr)                              \
    (camellia_128_ecb)                              \
    (camellia_128_ofb)                              \
    (camellia_192_cbc)                              \
    (camellia_192_cfb)                              \
    (camellia_192_cfb1)                             \
    (camellia_192_cfb8)                             \
    (camellia_192_ctr)                              \
    (camellia_192_ecb)                              \
    (camellia_192_ofb)                              \
    (camellia_256_cbc)                              \
    (camellia_256_cfb)                              \
    (camellia_256_cfb1)                             \
    (camellia_256_cfb8)                             \
    (camellia_256_ctr)                              \
    (camellia_256_ecb)                              \
    (camellia_256_ofb)                              \
    (camellia128)                                   \
    (camellia192)                                   \
    (camellia256)                                   \
    (chacha20)                                      \
    (des3)                                          \
    (sm4)                                           \
    (sm4_cbc)                                       \
    (sm4_cfb)                                       \
    (sm4_ctr)                                       \
    (sm4_ecb)                                       \
    (sm4_ofb)

// clang-format on

namespace nx::crypto {

struct encrypt_tag_t {
};
struct decrypt_tag_t {
};

NXAPI extern decrypt_tag_t decrypt;
NXAPI extern encrypt_tag_t encrypt;

#define DECLARE_CIPHER(r, data, alo)                                           \
    NXAPI int BOOST_PP_CAT(alo, _size)(                                        \
        decrypt_tag_t, const void* input, int size);                           \
    NXAPI int BOOST_PP_CAT(alo, _size)(                                        \
        encrypt_tag_t, const void* input, int size);                           \
    NXAPI int alo(void* out, decrypt_tag_t, const void* input, int size,       \
        const void* key, const void* iv);                                      \
    NXAPI int alo(void* out, encrypt_tag_t, const void* input, int size,       \
        const void* key, const void* iv);                                      \
    NXAPI std::string alo(decrypt_tag_t, const void* input, int size,          \
        const void* key, const void* iv);                                      \
    NXAPI std::string alo(encrypt_tag_t, const void* input, int size,          \
        const void* key, const void* iv);                                      \
    NXAPI std::string alo(encrypt_tag_t, const std::string& input,             \
        const void* key, const void* iv);                                      \
    NXAPI std::string alo(decrypt_tag_t, const std::string& input,             \
        const void* key, const void* iv);

BOOST_PP_SEQ_FOR_EACH(DECLARE_CIPHER, ~, NX_CRYPTO_CIPHER_ALGORITHMS())

#undef DECLARE_CIPHER

} // namespace nx::crypto

#endif