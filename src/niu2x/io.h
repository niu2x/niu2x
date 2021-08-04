#ifndef NX_IO_H
#define NX_IO_H

#include <fstream>

#include <niu2x/misc/ringbuffer.h>
#include <niu2x/misc/rw_status.h>
#include <niu2x/utils.h>

#if defined(_WIN32) || defined(_WIN64)
    #pragma warning(disable : 4275)
    #pragma warning(disable : 4251)
#endif

namespace nx::io {

void API read_file(const char *pathname, std::vector<uint8_t> &output);

using status = misc::rw_status;

template <class Elem>
class base_source {
public:
    base_source() { }
    virtual ~base_source() { }
    status get_elem(Elem *elem) { return get(elem, 1, nullptr); }
    virtual status get(Elem* output, size_t max, size_t* osize) = 0;
};

template <class Elem>
class base_sink {
public:
    base_sink() { }
    virtual ~base_sink() { }
    status put_elem(const Elem& elem) {
        return put(&elem, 1, nullptr);
    }
    virtual status put(const Elem* output, size_t isize, size_t* osize) = 0;
};

template <class IE, class OE>
class base_filter {
public:
    using from_t = IE;
    using to_t = OE;

    base_filter() { }
    virtual ~base_filter() { }

    virtual status cvt(const IE* input, size_t isize, size_t* consumed_isize,
        OE* output, size_t max_osize, size_t* osize)
        = 0;
};



namespace sink {
    template <class Elem, class Imp>
    class adapter: public base_sink<Elem> {};

    template <>
    class API adapter<uint8_t, std::ostream>: public base_sink<uint8_t> {
    public:
        adapter(std::ostream& delegate)
        :delegate_(delegate){}
        virtual ~adapter() {}
        virtual status put(
            const uint8_t* output, size_t isize, size_t* osize) override
        {
            delegate_.write(reinterpret_cast<const char*>(output), isize);
            NX_ASSERT(delegate_.good(), "adapter<uint8_t, std::ostream> write failed");

            if (osize)
                *osize = isize;
            return status::ok;
        }

        adapter(const adapter&) = default;
        adapter& operator=(const adapter&) = default;

    private:
        std::ostream &delegate_;
    };

    class API file : public base_sink<uint8_t>, private noncopyable {
    public:
        file(const char *pathname);
        virtual ~file();
        virtual status put(
            const uint8_t* output, size_t isize, size_t* osize) override;

    private:
        std::ofstream f_stream_;
        adapter<uint8_t, std::ostream> delegate_;
    };

    template <class Elem>
    class adapter<Elem, std::vector<Elem>> : public base_sink<Elem> {
    public:
        adapter(std::vector<Elem>& delegate)
        : delegate_(delegate)
        {
        }
        virtual ~adapter() { }
        virtual status put(
            const uint8_t* output, size_t isize, size_t* osize) override
        {
            delegate_.insert(delegate_.end(), output, output + isize);
            if (osize)
                *osize = isize;
            return status::ok;
        }

        adapter(const adapter&) = default;
        adapter& operator=(const adapter&) = default;

    private:
        std::vector<Elem>& delegate_;
    };

    extern API adapter<uint8_t, std::ostream> cout;
    extern API adapter<uint8_t, std::ostream> cerr;
}



template <class Elem, size_t CHUNK = 1_k>
void pipe(base_source<Elem>& src, base_sink<Elem>& dst)
{
    std::vector<Elem> buffer;
    buffer.resize(CHUNK);

    size_t readn = 0;
    while(!readn){
        status s;
        while ((s = src.get(buffer.data(), CHUNK, &readn)) == status::again)
            ;
        if(s == status::eof) break;

        auto* ptr = buffer.data();
        while(readn){
            size_t writen = 0;
            while ((s = dst.put(ptr, readn, &writen)) == status::again)
                ;
            if(s == status::eof) break;
            readn -= writen;
            ptr += writen;
        }
    }
}

namespace details {

// template <class E, size_t CHUNK>
// bool ringbuffer_near_full(const misc::ringbuffer<E, CHUNK>& rb)
// {
//     return rb.size() >= CHUNK - 1 - 1;
// }

template <class IE, class OE, size_t CHUNK = 1_k>
void pipe(base_source<IE>& src, base_filter<IE, OE>& flt, base_sink<OE>& dst)
{
    misc::ringbuffer<IE, CHUNK> ibuf;
    misc::ringbuffer<OE, CHUNK> obuf;

    bool src_eof = false;
    bool dst_eof = false;
    bool filter_eof = false;

    while (
        (!src_eof || ibuf.size() || obuf.size() || !filter_eof) && (!dst_eof)) {

        while (!ibuf.full() && !src_eof) {
            status s;
            size_t readn = 0;
            auto m = ibuf.continuous_slots();
            while ((s = src.get(m.base, m.size, &readn)) == status::again)
                ;
            src_eof = s == status::eof;
            if (!src_eof) {
                ibuf.push(readn);
            }
        }

        while (!obuf.full() && !ibuf.empty()) {
            auto im = ibuf.continuous_elems();
            auto om = obuf.continuous_slots();

            size_t readn = 0;
            size_t writen = 0;
            status s;

            while ((s = flt.cvt(
                        im.base, im.size, &readn, om.base, om.size, &writen))
                == status::again) {
                ibuf.normalize();
                obuf.normalize();

                im = ibuf.continuous_elems();
                om = obuf.continuous_slots();
            }

            filter_eof = s == status::eof;
            if (!filter_eof) {
                ibuf.pop(readn);
                obuf.push(writen);
            }
        }

        int filter_try = 2;
        while (filter_try && src_eof && ibuf.empty() && !obuf.full()
            && !filter_eof) {
            auto om = obuf.continuous_slots();
            size_t writen = 0;
            status s;

            while ((s = flt.cvt(nullptr, 0, nullptr, om.base, om.size, &writen))
                == status::again) {
                obuf.normalize();
                om = obuf.continuous_slots();
                if (!(--filter_try))
                    break;
            }
            filter_eof = s == status::eof;
            if (!filter_eof) {
                obuf.push(writen);
            }
        }

        while (!obuf.empty() && !dst_eof) {
            auto m = obuf.continuous_elems();
            size_t writen = 0;
            status s;
            while ((s = dst.put(m.base, m.size, &writen)) == status::again)
                ;
            dst_eof = s == status::eof;
            if (!dst_eof) {
                obuf.pop(writen);
            }
        }
    }
}
} // namespace details

template <class IE, class OE, class FILTER, size_t CHUNK = 1_k>
void pipe(base_source<IE>& src, FILTER p_flt, base_sink<OE>& dst)
{
    base_filter<IE, OE>& flt = p_flt;
    details::pipe(src, flt, dst);
}

namespace source {

template <class Elem> 
class array : public base_source<Elem> {
public:
    array(const Elem* const base, size_t size)
    : base_(base)
    , size_(size)
    , read_index_(0)
    {
    }
    virtual ~array() { }

    virtual status get(Elem* output, size_t max,  size_t* osize) override
    {
        size_t rest = (size_ - read_index_);
        if (rest == 0)
            return status::eof;

        size_t readn = min(rest, max);
        memcpy(output, base_ + read_index_, sizeof(Elem) * readn);

        read_index_ += readn;
        if (osize)
            *osize = readn;
        return status::ok;
    }

    array(const array&) = default;
    array& operator=(const array&) = default;

private:
    const Elem* const base_;
    const size_t size_;
    size_t read_index_;
};

using bytes = array<uint8_t>;

template <class Elem, class Imp>
class adapter: public base_source<Elem> {};

template <>
class API adapter<uint8_t, std::istream>: public base_source<uint8_t> {
public:
    adapter(std::istream& delegate)
    :delegate_(delegate){}
    virtual ~adapter() {}
    virtual status get(uint8_t* output, size_t max, size_t* osize) override {
            delegate_.read (reinterpret_cast<char*>(output),max);
            size_t readn;
            if (delegate_)
                readn = max;
            else
                readn = delegate_.gcount();
            if(osize) *osize = readn;

            return readn ? status::ok: status::eof;
        }

        adapter(const adapter&) = default;
        adapter& operator=(const adapter&) = default;

    private:
        std::istream& delegate_;
};

class API file : public base_source<uint8_t>, private noncopyable {
public:
    file(const char *pathname);
    virtual ~file();
    virtual status get(uint8_t* output, size_t max, size_t* osize) override ;

private:
    std::ifstream f_stream_;
    adapter<uint8_t, std::istream> delegate_;
};

extern API adapter<uint8_t, std::istream> cin;

}

namespace filter {

template <class IE, class OE>
class simple_filter : public base_filter<IE, OE> {
public:
    using converter = std::function<OE(const IE& c)>;

    simple_filter(const converter& cvt)
    : converter_(cvt)
    {
    }
    virtual ~simple_filter() { }

    virtual status cvt(const IE* input, size_t isize, size_t* consumed_isize,
        OE* output, size_t max_osize, size_t* osize) override
    {
        unused(isize, max_osize);

        if (isize) {
            if (consumed_isize)
                *consumed_isize = 1;
            if (osize)
                *osize = 1;
            output[0] = converter_(input[0]);
            return status::ok;
        }
        return status::eof;
    }

private:
    converter converter_;
};

template <class IE, class ME, class OE, size_t CHUNK = 1_k>
class concat_filter : public base_filter<IE, OE> {
public:
    concat_filter(base_filter<IE, ME>& first, base_filter<ME, OE>& second)
    : first_(first)
    , second_(second)
    , tmp_size_(0)
    , first_eof_(false)
    {
    }
    ~concat_filter() { }

    virtual status cvt(const IE* input, size_t isize, size_t* consumed_isize,
        OE* output, size_t max_osize, size_t* osize) override
    {

        size_t first_readn;
        size_t first_writen;
        status first_status;

        size_t second_readn;
        size_t second_writen;
        status second_status;

        if (tmp_size_) {
            first_status = status::ok;
            first_writen = tmp_size_;
            first_readn = 0;
        } else {
            if (first_eof_) {
                first_status = status::eof;
            } else {
                first_status = first_.cvt(
                    input, isize, &first_readn, tmp_, CHUNK, &first_writen);
            }
        }

        switch (first_status) {
            case status::ok: {
                second_status = second_.cvt(tmp_, first_writen, &second_readn,
                    output, max_osize, &second_writen);

                switch (second_status) {
                    case status::ok: {
                        if (consumed_isize)
                            *consumed_isize = first_readn;
                        if (osize)
                            *osize = second_writen;
                        tmp_size_ = first_writen - second_readn;
                        memcpy(tmp_, tmp_ + second_readn,
                            sizeof(tmp_size_) * sizeof(ME));
                        return status::ok;
                    }

                    case status::again: {
                        tmp_size_ = first_writen;
                        return status::again;
                    }

                    case status::eof: {
                        // never goto here
                        break;
                    }
                }
            }

            case status::again: {
                return status::again;
            }

            case status::eof: {
                first_eof_ = true;

                size_t second_writen;
                second_status = second_.cvt(
                    nullptr, 0, nullptr, output, max_osize, &second_writen);

                switch (second_status) {
                    case status::ok: {
                        if (consumed_isize)
                            *consumed_isize = 0;
                        if (osize)
                            *osize = second_writen;
                        return status::ok;
                    }

                    case status::again: {
                        return status::again;
                    }

                    case status::eof: {
                        return status::eof;
                    }
                }
            }
        }
        return status::eof;
    }

private:
    base_filter<IE, ME>& first_;
    base_filter<ME, OE>& second_;
    bool first_eof_;
    ME tmp_[CHUNK];
    size_t tmp_size_;
};

class API hex_encode_t : public base_filter<uint8_t, uint8_t> {
public:
    hex_encode_t() { }
    ~hex_encode_t() { }
    virtual status cvt(const uint8_t* input, size_t isize,
        size_t* consumed_isize, uint8_t* output, size_t max_osize,
        size_t* osize) override;
};

class API hex_decode_t : public base_filter<uint8_t, uint8_t> {
public:
    hex_decode_t() { }
    ~hex_decode_t() { }
    virtual status cvt(const uint8_t* input, size_t isize,
        size_t* consumed_isize, uint8_t* output, size_t max_osize,
        size_t* osize) override;
};

class API zlib_compress_t : public base_filter<uint8_t, uint8_t> {
public:
    zlib_compress_t(int level = -1);
    ~zlib_compress_t();

    zlib_compress_t(const zlib_compress_t &other);
    zlib_compress_t& operator=(const zlib_compress_t &other) = delete;

    virtual status cvt(const uint8_t* input, size_t isize,
        size_t* consumed_isize, uint8_t* output, size_t max_osize,
        size_t* osize) override;
private:
    struct context;
    std::unique_ptr<context> ctx_;
    int level_;
};

class API zlib_uncompress_t : public base_filter<uint8_t, uint8_t> {
public:
    zlib_uncompress_t();
    ~zlib_uncompress_t();

    zlib_uncompress_t(const zlib_uncompress_t& other);
    zlib_uncompress_t& operator=(const zlib_uncompress_t& other) = delete;

    virtual status cvt(const uint8_t* input, size_t isize,
        size_t* consumed_isize, uint8_t* output, size_t max_osize,
        size_t* osize) override;

private:
    struct context;
    std::unique_ptr<context> ctx_;
};

template <class Elem>
class one_t : public base_filter<Elem, Elem> {
public:
    one_t() { }
    ~one_t() { }
    virtual status cvt(const Elem* input, size_t isize, size_t* consumed_isize,
        Elem* output, size_t max_osize, size_t* osize) override
    {
        if (!isize)
            return status::eof;

        size_t readn = min(isize, max_osize);

        if (readn) {
            if (consumed_isize)
                *consumed_isize = readn;
            if (osize)
                *osize = readn;
            memcpy(output, input, readn * sizeof(Elem));
            return status::ok;
        } else {
            return status::again;
        }
    }
};

extern API simple_filter<uint8_t, uint8_t> lower;
extern API simple_filter<uint8_t, uint8_t> upper;
extern API simple_filter<uint8_t, uint8_t> inc;
extern API hex_encode_t hex_encode;
extern API hex_decode_t hex_decode;
extern API one_t<uint8_t> one;
extern API zlib_compress_t zlib_compress;
extern API zlib_uncompress_t zlib_uncompress;

// template <class FILTER>
// FILTER get(const char* name)
// {
//     if (strcmp(name, "lower"))
// }

} // namespace filter

template <class IE, class OE, size_t CHUNK = 1_k>
void operator|(base_source<IE>& src, base_sink<OE>& dst)
{
    pipe(src, dst);
}

template <class IE, class ME, class OE>
filter::concat_filter<IE, ME, OE> operator|(
    base_filter<IE, ME>& a, base_filter<ME, OE>& b)
{
    return filter::concat_filter(a, b);
}

} // namespace nx::io

#endif