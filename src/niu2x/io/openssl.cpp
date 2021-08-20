#include <niu2x/io.h>

namespace nx::io {

digest::digest(const char* algorithm)
: ctx_(nullptr)
, algorithm_(nullptr)
, state_(uninit)
{
    ctx_ = EVP_MD_CTX_new();
    NX_ASSERT(ctx_ != nullptr, "invalid ctx_");

    algorithm_ = EVP_get_digestbyname(algorithm);
    NX_ASSERT(algorithm_ != nullptr, "invalid algorithm_");
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
        EVP_DigestInit_ex2(ctx_, algorithm_, nullptr);
        state_ = inited;
    }

    if (state_ == inited) {
        if (isize == 0) {
            state_ = finishing;
        } else {
            EVP_DigestUpdate(ctx_, in, isize);
            readn = isize;
        }
    }

    if (state_ == finishing) {
        if (max_osize >= EVP_MAX_MD_SIZE) {
            unsigned int digest_len;
            EVP_DigestFinal_ex(ctx_, out, &digest_len);
            writen = digest_len;
            NX_LOG_T("digest_len %lu", digest_len);
            state_ = finished;
        } else {
            NX_LOG_E("no enough space for digest");
        }
    }
}

} // namespace nx::io