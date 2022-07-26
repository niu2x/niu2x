#include <niu2x/crypto/cipher.h>

#include <niu2x/build.h>
#if defined(OpenSSL_FOUND)

    #include <string.h>

    #define OPENSSL_NO_DEPRECATED 1
    #include <openssl/evp.h>
    #include <openssl/err.h>

    #include <niu2x/utils.h>

    #include "common.h"

    #define OPENSSL_ERR_MSG() ERR_reason_error_string(ERR_peek_last_error())

namespace nx::crypto {

static int cipher(const char* p_algorithm_name, void* out, int mode,
    const void* input, int size, const void* key, const void* iv)
{
    const int max = 32;
    char alg_name[max];
    strncpy(alg_name, p_algorithm_name, max);
    char* ptr = alg_name;
    while (*ptr) {
        if (*ptr == '_')
            *ptr = '-';
        ++ptr;
    }

    auto* algorithm = EVP_get_cipherbyname(alg_name);
    NX_FAIL_COND_V(!algorithm, -E_CRYPTO_ALGORITHM);

    auto* ctx = EVP_CIPHER_CTX_new();
    NX_FAIL_COND_V(!ctx, -E_CRYPTO_CONTEXT);

    NX_FUNC_AT_EXIT(ctx_free, ([ctx]() { EVP_CIPHER_CTX_free(ctx); }));

    EVP_CipherInit_ex2(
        ctx, algorithm, (const uint8_t*)key, (const uint8_t*)iv, mode, nullptr);

    int outlen = 0;
    int success = EVP_CipherUpdate(
        ctx, (uint8_t*)out, &outlen, (const uint8_t*)input, size);

    NX_FAIL_COND_V(!success, -E_CRYPTO_UPDATE);

    int tail_len = 0;
    success = EVP_CipherFinal_ex(ctx, (uint8_t*)out + outlen, &tail_len);
    NX_FAIL_COND_V(!success, -E_CRYPTO_FINAL);

    return outlen + tail_len;
}

#define DEFINE_SIZE_FUNC(name)                                                 \
    int name##_size(decrypt_tag_t tag, const void* input, int size)            \
    {                                                                          \
        unused(tag, input);                                                    \
        return EVP_MAX_BLOCK_LENGTH + size;                                    \
    }                                                                          \
    int name##_size(encrypt_tag_t tag, const void* input, int size)            \
    {                                                                          \
        unused(tag, input);                                                    \
        return EVP_MAX_MD_SIZE + size;                                         \
    }

#define DEFINE_CRYPTO_FUNC(name)                                               \
    int name(void* out, decrypt_tag_t tag, const void* input, int size,        \
        const void* key, const void* iv)                                       \
    {                                                                          \
        unused(tag);                                                           \
        return cipher(#name, out, 0, input, size, key, iv);                    \
    }                                                                          \
    int name(void* out, encrypt_tag_t tag, const void* input, int size,        \
        const void* key, const void* iv)                                       \
    {                                                                          \
        unused(tag);                                                           \
        return cipher(#name, out, 1, input, size, key, iv);                    \
    }

#define DEFINE_CIPHER_STR_FUNC(name)                                           \
    std::string name(encrypt_tag_t tag, const void* input, int size,           \
        const void* key, const void* iv)                                       \
    {                                                                          \
        unused(tag);                                                           \
        int buffer_size = name##_size(tag, input, size);                       \
        uint8_t* buffer = new uint8_t[buffer_size];                            \
        int out_size = name(buffer, tag, input, size, key, iv);                \
        NX_THROW_COND_MSG(                                                     \
            out_size < 0, std::string("fail: ") + std::to_string(out_size));   \
        std::string result(buffer, buffer + out_size);                         \
        delete[] buffer;                                                       \
        return result;                                                         \
    }                                                                          \
    std::string name(decrypt_tag_t tag, const void* input, int size,           \
        const void* key, const void* iv)                                       \
    {                                                                          \
        unused(tag);                                                           \
        int buffer_size = name##_size(tag, input, size);                       \
        uint8_t* buffer = new uint8_t[buffer_size];                            \
        int out_size = name(buffer, tag, input, size, key, iv);                \
        NX_THROW_COND_MSG(out_size < 0,                                        \
            std::string("crypto fail: ") + std::to_string(out_size));          \
        std::string result(buffer, buffer + out_size);                         \
        delete[] buffer;                                                       \
        return result;                                                         \
    }

#define DEFINE_CIPHER_STR2_FUNC(name)                                          \
    std::string name(encrypt_tag_t tag, const std::string& input,              \
        const void* key, const void* iv)                                       \
    {                                                                          \
        return name(tag, input.c_str(), input.size(), key, iv);                \
    }                                                                          \
    std::string name(decrypt_tag_t tag, const std::string& input,              \
        const void* key, const void* iv)                                       \
    {                                                                          \
        return name(tag, input.c_str(), input.size(), key, iv);                \
    }

#define DEFINE_DIGEST(r, data, name)                                           \
    DEFINE_SIZE_FUNC(name)                                                     \
    DEFINE_CRYPTO_FUNC(name)                                                   \
    DEFINE_CIPHER_STR_FUNC(name)                                               \
    DEFINE_CIPHER_STR2_FUNC(name)

BOOST_PP_SEQ_FOR_EACH(DEFINE_DIGEST, ~, NX_CRYPTO_CIPHER_ALGORITHMS())

struct encrypt_tag_t encrypt;
struct decrypt_tag_t decrypt;

} // namespace nx::crypto

#endif