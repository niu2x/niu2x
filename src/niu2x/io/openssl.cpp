#include <niu2x/io.h>

#include <string.h>

namespace nx::io {

digest::digest(const char* algorithm)
: ctx_(nullptr)
, algorithm_(nullptr)
, state_(uninit)
{
    ctx_ = EVP_MD_CTX_new();
    NX_ASSERT(ctx_ != nullptr, "invalid digest ctx_");

    algorithm_ = EVP_get_digestbyname(algorithm);
    NX_ASSERT(algorithm_ != nullptr, "invalid digest algorithm_");
}

digest::~digest() { EVP_MD_CTX_free(ctx_); }

void digest::reset()
{
    EVP_MD_CTX_reset(ctx_);
    state_ = uninit;
}

void digest::cvt(const uint8_t* in, size_t isize, uint8_t* out,
    size_t max_osize, size_t& readn, size_t& writen)
{

    writen = readn = 0;

    if (state_ == uninit) {
        NX_LOG_T("digest::cvt EVP_DigestInit_ex2");
        EVP_DigestInit_ex2(ctx_, algorithm_, nullptr);
        state_ = inited;
    }

    if (state_ == inited) {
        if (isize == 0) {
            state_ = finishing;
        } else {
            NX_LOG_T("digest::cvt EVP_DigestUpdate %p %p %lu", ctx_, in, isize);
            EVP_DigestUpdate(ctx_, in, isize);
            readn = isize;
        }
    }

    if (state_ == finishing) {
        if (max_osize >= EVP_MAX_MD_SIZE) {
            unsigned int digest_len;
            NX_LOG_T("digest::cvt EVP_DigestFinal_ex");
            EVP_DigestFinal_ex(ctx_, out, &digest_len);
            writen = digest_len;
            state_ = finished;
        } else {
            NX_LOG_E("no enough space for digest");
        }
    }
}

cipher::cipher(
    int mode, const char* algorithm, const uint8_t key[], const uint8_t iv[])
: mode_(mode)
{
    ctx_ = EVP_CIPHER_CTX_new();
    NX_ASSERT(ctx_ != nullptr, "invalid digest ctx_");

    algorithm_ = EVP_get_cipherbyname(algorithm);
    NX_ASSERT(algorithm_ != nullptr, "invalid digest algorithm_");

    memcpy(key_, key, EVP_CIPHER_key_length(algorithm_));
    memcpy(iv_, iv, EVP_CIPHER_iv_length(algorithm_));
}

cipher::~cipher() { EVP_CIPHER_CTX_free(ctx_); }

void cipher::reset()
{
    EVP_CIPHER_CTX_reset(ctx_);
    state_ = uninit;
}

void cipher::cvt(const uint8_t* in, size_t isize, uint8_t* out,
    size_t max_osize, size_t& readn, size_t& writen)
{

    writen = readn = 0;

    if (state_ == uninit) {

        NX_LOG_T("cipher::cvt EVP_CipherInit_ex2");
        EVP_CipherInit_ex2(ctx_, algorithm_, key_, iv_, mode_, nullptr);

        state_ = inited;
    }

    if (state_ == inited) {
        if (isize == 0) {
            state_ = finishing;
        } else {
            isize = NX_MIN(isize, max_osize - EVP_MAX_BLOCK_LENGTH);
            int iwriten;
            NX_LOG_T("cipher::cvt EVP_CipherUpdate %p %lu", in, isize);
            int status = EVP_CipherUpdate(ctx_, out, &iwriten, in, isize);
            NX_ASSERT(status != 0,
                std::string("EVP_CipherUpdate fail ") + std::to_string(status));
            writen = iwriten;
            readn = isize;
        }
    }

    if (state_ == finishing) {
        if (max_osize >= EVP_MAX_BLOCK_LENGTH) {
            int iwriten;
            NX_LOG_T("cipher::cvt EVP_CipherFinal_ex");
            EVP_CipherFinal_ex(ctx_, out, &iwriten);
            writen = iwriten;
            state_ = finished;
        } else {
            NX_LOG_E("no enough space for EVP_CipherFinal_ex");
        }
    }
}

} // namespace nx::io