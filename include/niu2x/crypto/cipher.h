#ifndef NX_CRYPTO_CIPHER_H
#define NX_CRYPTO_CIPHER_H

#include <string>
#include <boost/preprocessor.hpp>
#include <niu2x/utils.h>
#include <niu2x/api.h>

// clang-format off
#define NX_CRYPTO_CIPHER_ALGORITHMS()          \
                        (aes_256_cbc)

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
        const void* key, const void* iv);

BOOST_PP_SEQ_FOR_EACH(DECLARE_CIPHER, ~, NX_CRYPTO_CIPHER_ALGORITHMS())

#undef DECLARE_CIPHER

} // namespace nx::crypto

#endif