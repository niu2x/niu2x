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

template <class Elem> class base_source : private noncopyable {
public:
    base_source() { }
    virtual ~base_source() { }
    status get_elem(Elem *elem) { return get(elem, 1, nullptr); }
    virtual status get(Elem* output, size_t max, size_t* osize) = 0;
};

template <class Elem> class base_sink : private noncopyable {
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

template <class Elem> 
class bufsource : public base_source<Elem> {
public:
    bufsource(const Elem* const base, size_t size)
    : base_(base)
    , size_(size)
    , read_index_(0)
    {
    }
    virtual ~bufsource() { }

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

private:
    const Elem* const base_;
    const size_t size_;
    size_t read_index_;
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
        virtual status put(const uint8_t* output, size_t isize, size_t* osize) {
            delegate_.write(reinterpret_cast<const char*>(output), isize / 2 + 1);
            if (osize)
                *osize = isize / 2 + 1;
            return status::ok;
        }
    private:
        std::ostream &delegate_;
    };

    class API file: public adapter<uint8_t, std::ostream> {
    public:
        file(const char *pathname);
        virtual ~file();
    private:
        std::ofstream f_stream_;
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

template <class IE, class OE, class FILTER, size_t CHUNK = 1_k>
void pipe(base_source<IE>& src, FILTER flt, base_sink<OE>& dst)
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
                == status::again)
                ;
            filter_eof = s == status::eof;
            if (!filter_eof) {
                ibuf.pop(readn);
                obuf.push(writen);
            }
        }

        while (src_eof && ibuf.empty() && !obuf.full() && !filter_eof) {
            auto om = obuf.continuous_slots();

            size_t writen = 0;
            status s;

            while ((s = flt.cvt(nullptr, 0, nullptr, om.base, om.size, &writen))
                == status::again)
                ;
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



using byte_source = bufsource<uint8_t>;

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

template <class FIRST_FILTER, class SECOND_FILTER, size_t CHUNK = 1_k>
class concat_filter : public base_filter<typename FIRST_FILTER::from_t,
                          typename SECOND_FILTER::to_t> {
public:
    using from_t = typename FIRST_FILTER::from_t;
    using to_t = typename SECOND_FILTER::to_t;

    using middle_t = typename FIRST_FILTER::to_t;

    concat_filter(const FIRST_FILTER& first, const SECOND_FILTER& second)
    : first_(first)
    , second_(second)
    , tmp_size_(0)
    , first_eof_(false)
    {
    }
    ~concat_filter() { }

    virtual status cvt(const from_t* input, size_t isize,
        size_t* consumed_isize, to_t* output, size_t max_osize,
        size_t* osize) override
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
                            sizeof(tmp_size_) * sizeof(middle_t));
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
    FIRST_FILTER first_;
    SECOND_FILTER second_;
    bool first_eof_;
    middle_t tmp_[CHUNK];
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

extern API simple_filter<uint8_t, uint8_t> lower;
extern API simple_filter<uint8_t, uint8_t> upper;
extern API simple_filter<uint8_t, uint8_t> inc;
extern API hex_encode_t hex_encode;

class zlib_compress_t: public base_filter<uint8_t, uint8_t> {
public:
    zlib_compress_t();
    ~zlib_compress_t();

    zlib_compress_t(const zlib_compress_t &other);
    zlib_compress_t& operator=(const zlib_compress_t &other) = delete;

    virtual status cvt(const uint8_t* input, size_t isize,
        size_t* consumed_isize, uint8_t* output, size_t max_osize,
        size_t* osize) override;
private:
    struct context;
    context *ctx_;
};


} // namespace filter

template <class IE, class OE, size_t CHUNK = 1_k>
void operator|(base_source<IE>& src, base_sink<OE>& dst)
{
    pipe(src, dst);
}

template <class FIRST_FILTER, class SECOND_FILTER>

std::enable_if_t<
    std::is_same_v<typename FIRST_FILTER::to_t, typename SECOND_FILTER::from_t>,
    filter::concat_filter<FIRST_FILTER, SECOND_FILTER>>
operator|(const FIRST_FILTER& a, const SECOND_FILTER& b)
{
    return filter::concat_filter(a, b);
}

} // namespace nx::io

#endif