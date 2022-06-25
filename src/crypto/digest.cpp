#include <niu2x/crypto/digest.h>

#include <string.h>

#define OPENSSL_NO_DEPRECATED 1
#include <openssl/evp.h>
#include <openssl/err.h>

#include <niu2x/utils.h>

#include "common.h"

namespace nx::crypto {

#define DEFINE_SIZE_FUNC(name)                                                 \
    int name##_size(const void* input, int len)                                \
    {                                                                          \
        unused(input, len);                                                    \
        return EVP_MAX_MD_SIZE;                                                \
    }

#define DEFINE_DIGEST_FUNC(name)                                               \
    int name(void* out, const void* input, int size)                           \
    {                                                                          \
        const int max = 32;                                                    \
        char alg_name[max];                                                    \
        strncpy(alg_name, #name, max);                                         \
        char* ptr = alg_name;                                                  \
        while (*ptr) {                                                         \
            if (*ptr == '_')                                                   \
                *ptr = '-';                                                    \
            ++ptr;                                                             \
        }                                                                      \
        auto* algorithm = EVP_get_digestbyname(alg_name);                      \
        NX_FAIL_COND_V(algorithm == nullptr, -E_ALO);                          \
        auto* ctx = EVP_MD_CTX_new();                                          \
        NX_FAIL_COND_V(ctx == nullptr, -E_CTX);                                \
        EVP_DigestInit_ex2(ctx, algorithm, nullptr);                           \
        EVP_DigestUpdate(ctx, input, size);                                    \
        uint32_t out_size;                                                     \
        EVP_DigestFinal_ex(ctx, (uint8_t*)out, &out_size);                     \
        EVP_MD_CTX_free(ctx);                                                  \
        return out_size;                                                       \
    }

#define DEFINE_DIGEST(r, data, name)                                           \
    DEFINE_SIZE_FUNC(name)                                                     \
    DEFINE_DIGEST_FUNC(name)                                                   \
    DEFINE_CRYPTO_STR_FUNC(name)

BOOST_PP_SEQ_FOR_EACH(DEFINE_DIGEST, ~, NX_CRYPTO_DIGEST_ALGORITHMS())

} // namespace nx::crypto
