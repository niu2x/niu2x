#include "../io.h"

#include <string.h>

#define OPENSSL_NO_DEPRECATED 1
#include <openssl/evp.h>
#include <openssl/err.h>

#include <niu2x/assert.h>

#define OPENSSL_CHECK(condition, message)                                      \
    if (!(condition)) {                                                        \
        auto e = ERR_peek_last_error();                                        \
        NX_LOG_F("%s reason: %d %s", message, e, ERR_reason_error_string(e));  \
        NX_THROW(message);                                                     \
    }

namespace nx::io::filter {

digest::digest(const char* p_algorithm_anme)
{
    auto* algorithm = EVP_get_digestbyname(p_algorithm_anme);
    OPENSSL_CHECK(algorithm != nullptr, "invalid digest algorithm");

    auto* ctx = EVP_MD_CTX_new();
    OPENSSL_CHECK(ctx != nullptr, "invalid digest ctx");
    digest_ctx_ = ctx;

    EVP_DigestInit_ex2(ctx, algorithm, nullptr);
}

digest::~digest()
{
    auto* ctx = reinterpret_cast<EVP_MD_CTX*>(digest_ctx_);
    EVP_MD_CTX_free(ctx);
}

void digest::transform(ringbuf& rbuf, ringbuf& wbuf, bool upstream_eof)
{
    auto* ctx = reinterpret_cast<EVP_MD_CTX*>(digest_ctx_);

    auto input = rbuf.continuous_elems();
    if (input.size > 0) {
        EVP_DigestUpdate(ctx, input.base, input.size);
        rbuf.update_size(-input.size);
    }
    if (upstream_eof) {
        auto output = wbuf.continuous_slots();
        if (output.size >= EVP_MAX_MD_SIZE) {
            uint32_t digest_len;
            EVP_DigestFinal_ex(ctx, output.base, &digest_len);
            wbuf.update_size(digest_len);
        } else {
            NX_LOG_E("no enough space for digest");
        }
    }
}

} // namespace nx::io::filter
