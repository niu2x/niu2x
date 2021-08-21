#ifndef NX_IO_H
#define NX_IO_H

#include <iostream>
#include <functional>
#include <string>

#include <zlib.h>
#include <openssl/evp.h>

#include <niu2x/utils.h>
#include <niu2x/ringbuffer.h>

namespace nx::io {

using ringbuffer = nx::ringbuffer<uint8_t, 1023>;

class API source : private noncopyable {
public:
    source();
    virtual ~source() = 0;
    status get_elem(uint8_t& elem)
    {
        size_t osize;
        return get(&elem, 1, osize);
    }
    virtual status get(uint8_t* output, size_t max, size_t& osize) = 0;
};

class API sink : private noncopyable {
public:
    sink();
    virtual ~sink() = 0;
    status put_elem(uint8_t elem)
    {
        size_t osize;
        return put(&elem, 1, osize);
    }
    virtual status put(const uint8_t* output, size_t isize, size_t& osize) = 0;
};

class API source_proxy {
public:
    source_proxy(source* delegate)
    : delegate_(delegate)
    {
    }
    ~source_proxy() { }

    source_proxy(const source_proxy&) = default;
    source_proxy& operator=(const source_proxy&) = default;

    status get(uint8_t* output, size_t max, size_t& osize)
    {
        return delegate_->get(output, max, osize);
    }
    status get_elem(uint8_t& elem) { return delegate_->get_elem(elem); }

private:
    source* delegate_;
};

class API sink_proxy {
public:
    sink_proxy(sink* delegate)
    : delegate_(delegate)
    {
    }
    ~sink_proxy() { }
    sink_proxy(const sink_proxy&) = default;
    sink_proxy& operator=(const sink_proxy&) = default;
    status put(const uint8_t* output, size_t isize, size_t& osize)
    {
        return delegate_->put(output, isize, osize);
    }
    status put_elem(uint8_t elem) { return delegate_->put_elem(elem); }

private:
    sink* delegate_;
};

class API std_istream_adapter : public source {
public:
    std_istream_adapter(std::istream& delegate);
    virtual ~std_istream_adapter();
    virtual status get(uint8_t* output, size_t max, size_t& osize) override;

private:
    std::istream& delegate_;
};

class API std_ostream_adapter : public sink {
public:
    std_ostream_adapter(std::ostream& delegate);
    virtual ~std_ostream_adapter();
    virtual status put(
        const uint8_t* output, size_t max, size_t& osize) override;

private:
    std::ostream& delegate_;
};

class API inbuf : public source {
public:
    inbuf(const void* base, size_t size);
    virtual ~inbuf();
    virtual status get(uint8_t* output, size_t max, size_t& osize) override;

private:
    const_memref buffer_;
    size_t read_pos_;
};

class API null_sink : public sink {
public:
    null_sink() { }
    virtual ~null_sink() { }
    virtual status put(
        const uint8_t* output, size_t max, size_t& osize) override
    {
        unused(output);
        osize = max;
        return ok;
    }
};

class API vector : public sink {
public:
    vector(std::vector<uint8_t>&);
    virtual ~vector();
    virtual status put(
        const uint8_t* output, size_t max, size_t& osize) override;

private:
    std::vector<uint8_t>& backend_;
};

class API string : public sink {
public:
    string(std::string&);
    virtual ~string();
    virtual status put(
        const uint8_t* output, size_t max, size_t& osize) override;

private:
    std::string& backend_;
};

extern API sink_proxy null;
extern API sink_proxy cout;
extern API sink_proxy cerr;
extern API source_proxy cin;

class API filter : public sink {
public:
    filter();
    virtual ~filter();

    virtual void reset() = 0;

    void set_next(sink_proxy next);
    void set_next(filter* next);

    virtual status put(
        const uint8_t* output, size_t isize, size_t& osize) override;

    virtual void cvt(const uint8_t* in, size_t isize, uint8_t* out,
        size_t max_osize, size_t& readn, size_t& writen)
        = 0;

private:
    sink_proxy next_sink_;
    filter* next_filter_;

    enum class nt {
        none,
        sink,
        filter,
    };

    nt next_type_;
    ringbuffer buf_;
};

class API simple_filter : public filter {
public:
    using converter = std::function<uint8_t(uint8_t)>;
    simple_filter(const converter& p_converter);
    virtual ~simple_filter();
    virtual void reset() override;
    virtual void cvt(const uint8_t* in, size_t isize, uint8_t* out,
        size_t max_osize, size_t& readn, size_t& writen);

private:
    converter converter_;
};

class API hex_filter : public filter {
public:
    hex_filter();
    virtual ~hex_filter();
    virtual void reset() override;
    virtual void cvt(const uint8_t* in, size_t isize, uint8_t* out,
        size_t max_osize, size_t& readn, size_t& writen);
};

class API unhex_filter : public filter {
public:
    unhex_filter();
    virtual ~unhex_filter();
    virtual void reset() override;
    virtual void cvt(const uint8_t* in, size_t isize, uint8_t* out,
        size_t max_osize, size_t& readn, size_t& writen);
};

class API zlib_filter : public filter {
public:
    zlib_filter(int level = -1);
    virtual ~zlib_filter();

    virtual void reset() override;

    virtual void cvt(const uint8_t* in, size_t isize, uint8_t* out,
        size_t max_osize, size_t& readn, size_t& writen) override;

private:
    z_stream strm_;
    int level_;
};

class API unzlib_filter : public filter {
public:
    unzlib_filter();
    virtual ~unzlib_filter();

    virtual void reset() override;

    virtual void cvt(const uint8_t* in, size_t isize, uint8_t* out,
        size_t max_osize, size_t& readn, size_t& writen) override;

private:
    z_stream strm_;
};

class API digest : public filter {
public:
    digest(const char* algorithm);
    virtual ~digest();

    virtual void reset() override;
    virtual void cvt(const uint8_t* in, size_t isize, uint8_t* out,
        size_t max_osize, size_t& readn, size_t& writen) override;

private:
    EVP_MD_CTX* ctx_;
    const EVP_MD* algorithm_;
    enum {
        uninit,
        inited,
        finishing,
        finished,
    } state_;
};

class API cipher : public filter {
public:
    enum mode { decrypt = 0, encrypt = 1 };

    cipher(int mode, const char* algorithm, const uint8_t key[],
        const uint8_t iv[]);
    virtual ~cipher();

    virtual void reset() override;
    virtual void cvt(const uint8_t* in, size_t isize, uint8_t* out,
        size_t max_osize, size_t& readn, size_t& writen) override;

private:
    EVP_CIPHER_CTX* ctx_;
    const EVP_CIPHER* algorithm_;
    enum {
        uninit,
        inited,
        finishing,
        finished,
    } state_;

    uint8_t key_[EVP_MAX_KEY_LENGTH];
    uint8_t iv_[EVP_MAX_IV_LENGTH];

    int mode_;
};

class API filter_proxy {
public:
    filter_proxy(filter* delegate)
    : delegate_(delegate)
    {
    }
    ~filter_proxy() { }

    filter_proxy(const filter_proxy&) = default;
    filter_proxy& operator=(const filter_proxy&) = default;

    status put(const uint8_t* output, size_t max, size_t& osize)
    {
        return delegate_->put(output, max, osize);
    }

    void reset() { delegate_->reset(); }
    void set_next(sink_proxy next) { delegate_->set_next(next); }

    filter* delegate() noexcept { return delegate_; }

    // filter_proxy operator|(const filter_proxy& other) { }

private:
    filter* delegate_;
};

extern API filter_proxy one;
extern API filter_proxy lower;
extern API filter_proxy upper;
extern API filter_proxy hex;
extern API filter_proxy unhex;

API void pipe(source_proxy src, sink_proxy dst);
API void pipe(source_proxy src, filter_proxy dst);
API void pipe(source_proxy src, filter_proxy filter, sink_proxy dst);

API void pipe(
    source_proxy src, filter_proxy f0, filter_proxy f1, sink_proxy dst);

} // namespace nx::io

#endif