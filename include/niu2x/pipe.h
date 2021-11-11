#ifndef NX_PIPE_H
#define NX_PIPE_H

#include <iostream>
#include <functional>
#include <variant>
#include <string>

#include <boost/noncopyable.hpp>

#include <niu2x/api.h>
#include <niu2x/memory.h>
#include <niu2x/ringbuf.h>

namespace nx::pipe {

extern NXAPI memory_proxy mem;

enum NXAPI status {
    ok = 0,
    again,
    eof,
    fail,
};

class NXAPI source {
public:
    source(std::istream& stream);
    ~source();

    source(const source&) = default;
    source(source&&) = default;
    source& operator=(const source&) = default;
    source& operator=(source&&) = default;

    int read(void* data, size_t bytes);

private:
    std::variant<std::istream*> delegate_;
};

class NXAPI sink {
public:
    sink(std::ostream& stream);
    ~sink();

    sink(const sink&) = default;
    sink& operator=(const sink&) = default;

    int write(const void* data, size_t bytes);

private:
    std::variant<std::ostream*> delegate_;
};

namespace filter {

using ringbuf = nx::ringbuf<uint8_t, 4096>;

class NXAPI filter : private boost::noncopyable {
public:
    class proxy_t {
    public:
        proxy_t(filter* p_filter)
        : delegate_(p_filter)
        {
        }
        ~proxy_t() { }
        proxy_t(const proxy_t&) = default;
        proxy_t& operator=(const proxy_t&) = default;
        int read(void* data, size_t bytes)
        {
            return delegate_->read(data, bytes);
        }

        void set_upstream(source p_upstream)
        {
            delegate_->set_upstream(p_upstream);
        }
        void set_upstream(proxy_t p_upstream)
        {
            delegate_->set_upstream(p_upstream);
        }

    private:
        filter* delegate_;
    };

    filter();
    virtual ~filter();

    int read(void* data, size_t bytes);

    void set_upstream(source p_upstream);
    void set_upstream(proxy_t p_upstream);

    virtual bool transform(ringbuf&, ringbuf&, bool upstream_eof) = 0;

    proxy_t proxy() noexcept { return proxy_t(this); }

    operator proxy_t() noexcept { return proxy(); }

private:
    void read_from_upstream();
    int write_to_downstream(void* data, size_t bytes);

    std::variant<source, proxy_t, std::nullptr_t> upstream_;
    ringbuf rbuf_;
    ringbuf wbuf_;
    bool upstream_eof_;
    bool transform_eof_;
};

class NXAPI simple_filter : public filter {
public:
    virtual bool transform(ringbuf&, ringbuf&, bool upstream_eof);
    virtual uint8_t transform(uint8_t chr) = 0;
};

class NXAPI lower : public simple_filter {
public:
    virtual uint8_t transform(uint8_t chr) { return tolower(chr); }
};

class NXAPI upper : public simple_filter {
public:
    virtual uint8_t transform(uint8_t chr) { return toupper(chr); }
};

class NXAPI hex : public filter {
public:
    virtual bool transform(ringbuf&, ringbuf&, bool upstream_eof);
};

class NXAPI unhex : public filter {
public:
    unhex();
    virtual ~unhex() { }
    virtual bool transform(ringbuf&, ringbuf&, bool upstream_eof);

private:
    uint8_t buf_[2];
    uint8_t size_;
};

class NXAPI base64 : public filter {
public:
    virtual bool transform(ringbuf&, ringbuf&, bool upstream_eof);
};

class NXAPI unbase64 : public filter {
public:
    virtual bool transform(ringbuf&, ringbuf&, bool upstream_eof);
};

class NXAPI cut : public filter {
public:
    cut(uint8_t chr);
    virtual ~cut() { }
    virtual bool transform(ringbuf&, ringbuf&, bool upstream_eof);

private:
    uint8_t chr_;
};

class NXAPI zlib : public filter {
public:
    enum { default_level = 6 };

    zlib(int level = default_level);
    virtual ~zlib();

    virtual bool transform(ringbuf&, ringbuf&, bool upstream_eof);

private:
    void* zlib_ctx_;
};

class NXAPI unzlib : public filter {
public:
    unzlib();
    virtual ~unzlib();
    virtual bool transform(ringbuf&, ringbuf&, bool upstream_eof);

private:
    void* zlib_ctx_;
};

class NXAPI digest : public filter {
public:
    /**
     * @brief      { function_description }
     *
     * @param[in]  algorithm  The algorithm:
                                "blake2b512",
                                "blake2s256",
                                "md5",
                                "md5-sha1",
                                "sha1",
                                "sha224",
                                "sha256",
                                "sha3-224",
                                "sha3-256",
                                "sha3-384",
                                "sha3-512",
                                "sha384",
                                "sha512",
                                "sha512-224",
                                "sha512-256",
                                "shake128",
                                "shake256",
                                "sm3",
                                "ssl3-md5",
                                "ssl3-sha1",
     */
    digest(const char* algorithm);
    virtual ~digest();

    virtual bool transform(ringbuf&, ringbuf&, bool upstream_eof);

private:
    void* digest_ctx_;
};

class NXAPI cipher : public filter {
public:
    /**
     * @brief      { function_description }
     *
     * @param[in]  algorithm  The algorithm:
                                    "aes-128-cbc",
                                    "aes-128-cfb",
                                    "aes-128-cfb1",
                                    "aes-128-cfb8",
                                    "aes-128-ctr",
                                    "aes-128-ecb",
                                    "aes-128-ofb",
                                    "aes-192-cbc",
                                    "aes-192-cfb",
                                    "aes-192-cfb1",
                                    "aes-192-cfb8",
                                    "aes-192-ctr",
                                    "aes-192-ecb",
                                    "aes-192-ofb",
                                    "aes-256-cbc",
                                    "aes-256-cfb",
                                    "aes-256-cfb1",
                                    "aes-256-cfb8",
                                    "aes-256-ctr",
                                    "aes-256-ecb",
                                    "aes-256-ofb",
                                    "aes128",
                                    "aes192",
                                    "aes256",
                                    "aria-128-cbc",
                                    "aria-128-cfb",
                                    "aria-128-cfb1",
                                    "aria-128-cfb8",
                                    "aria-128-ctr",
                                    "aria-128-ecb",
                                    "aria-128-ofb",
                                    "aria-192-cbc",
                                    "aria-192-cfb",
                                    "aria-192-cfb1",
                                    "aria-192-cfb8",
                                    "aria-192-ctr",
                                    "aria-192-ecb",
                                    "aria-192-ofb",
                                    "aria-256-cbc",
                                    "aria-256-cfb",
                                    "aria-256-cfb1",
                                    "aria-256-cfb8",
                                    "aria-256-ctr",
                                    "aria-256-ecb",
                                    "aria-256-ofb",
                                    "aria128",
                                    "aria192",
                                    "aria256",
                                    "camellia-128-cbc",
                                    "camellia-128-cfb",
                                    "camellia-128-cfb1",
                                    "camellia-128-cfb8",
                                    "camellia-128-ctr",
                                    "camellia-128-ecb",
                                    "camellia-128-ofb",
                                    "camellia-192-cbc",
                                    "camellia-192-cfb",
                                    "camellia-192-cfb1",
                                    "camellia-192-cfb8",
                                    "camellia-192-ctr",
                                    "camellia-192-ecb",
                                    "camellia-192-ofb",
                                    "camellia-256-cbc",
                                    "camellia-256-cfb",
                                    "camellia-256-cfb1",
                                    "camellia-256-cfb8",
                                    "camellia-256-ctr",
                                    "camellia-256-ecb",
                                    "camellia-256-ofb",
                                    "camellia128",
                                    "camellia192",
                                    "camellia256",
                                    "chacha20",
                                    "des3",
                                    "sm4",
                                    "sm4-cbc",
                                    "sm4-cfb",
                                    "sm4-ctr",
                                    "sm4-ecb",
                                    "sm4-ofb",

     */

    enum { decrypt = 0, encrypt = 1 };

    cipher(const char* algorithm, int mode, const uint8_t key[],
        const uint8_t iv[]);
    virtual ~cipher();

    virtual bool transform(ringbuf&, ringbuf&, bool upstream_eof);

private:
    void* cipher_ctx_;
};

class NXAPI encrypt : public cipher {
public:
    encrypt(const char* algorithm, const uint8_t key[], const uint8_t iv[])
    : cipher(algorithm, cipher::encrypt, key, iv)
    {
    }
    virtual ~encrypt() { }
};

class NXAPI decrypt : public cipher {
public:
    decrypt(const char* algorithm, const uint8_t key[], const uint8_t iv[])
    : cipher(algorithm, cipher::decrypt, key, iv)
    {
    }
    virtual ~decrypt() { }
};

NXAPI filter::proxy_t operator|(
    filter::proxy_t p_source, filter::proxy_t p_filter);

}; // namespace filter

using filter_proxy = filter::filter::proxy_t;

NXAPI bool operator|(source p_source, sink p_sink);

NXAPI bool operator|(filter::filter::proxy_t p_filter, sink p_sink);

NXAPI filter_proxy operator|(source p_source, filter_proxy p_filter);

} // namespace nx::pipe

#endif