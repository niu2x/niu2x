#ifndef NX_IO_H
#define NX_IO_H

#include <niu2x/utils.h>
#include <niu2x/misc/rw_status.h>

#if defined(_WIN32) || defined(_WIN64)
    #pragma warning(disable : 4275)
#endif

namespace nx::io {

void API read_file(const char *pathname, std::vector<uint8_t> &output);

using status = misc::rw_status;

template <class Elem> class source : private noncopyable {
public:
    source() { }
    virtual ~source() { }
    status get_elem(Elem *elem) { return get(elem, 1, nullptr); }
    virtual status get(Elem* output, size_t max, size_t* osize) = 0;
};

template <class Elem> class sink : private noncopyable {
public:
    sink() { }
    virtual ~sink() { }
    status put_elem(const Elem& elem) {
        return put(&elem, 1, nullptr);
    }
    virtual status put(const Elem* output, size_t isize, size_t* osize) = 0;
};

template <class IE, class OE> 
class filter : private noncopyable {
public:
    filter() {}
    virtual ~filter() {}

    virtual status cvt(const IE* input
        , size_t isize
        , size_t *consumed_isize
        , OE* output
        , size_t max_osize
        , size_t* osize) = 0;
};

template<class IE, class OE, size_t CHUNK=512>
class filtered_source : public source<OE> {
public:
    filtered_source(source<IE> &src):src_(src), isize_(0), osize_(0) {}
    virtual ~filtered_source(){}

    virtual status get(OE* output, size_t max, size_t* osize) override
    {

        if(osize_) {
            auto readn = min(max, osize_);
        }

        // size_t rest = (size_ - read_index_);
        // if (rest == 0)
        //     return status::eof;
        // size_t readn = min(rest, max);
        // memcpy(output, base_ + read_index_, sizeof(Elem) * readn);
        // read_index_ += readn;
        // if (osize)
        //     *osize = readn;
        return status::ok;
    }

private:
    source<IE> &src_;
    IE ibuf_[CHUNK];
    size_t isize_;
    OE obuf_[CHUNK];
    size_t osize_;
};

template <class Elem> 
class bufsource : public source<Elem> {
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

using byte_source = bufsource<uint8_t>;





template <class Elem, class Imp>
class sink_adapter: public sink<Elem> {};


template <>
class API sink_adapter<uint8_t, std::ostream>: public sink<uint8_t> {
public:
    sink_adapter(std::ostream& delegate)
    :delegate_(delegate){}
    virtual ~sink_adapter() {}
    virtual status put(const uint8_t* output, size_t isize, size_t* osize) {
        delegate_.write (reinterpret_cast<const char *>(output), isize);
        if(osize) *osize = isize;
        return status::ok;
    }
private:
    std::ostream &delegate_;
}; 

template<class Elem>
void pipe(source<Elem> &src, sink<Elem> &dst, size_t chunk = 4_k) {
    std::vector<Elem> buffer;
    buffer.resize(chunk);

    size_t readn = 0;
    while(!readn){
        status s;
        while( (s=src.get(buffer.data(), chunk, &readn)) == status::again);
        if(s == status::eof) break;
        while(readn){
            size_t writen = 0;
            while((s=dst.put(buffer.data(), readn, &writen)) == status::again);
            if(s == status::eof) break;
            readn -= writen;
        }
    }
}

// template<class T>
// class buffer {
// public:
// 	buffer(T * const base, size_t size)
// 	:base_(base)
// 	,size_(size)
// 	{}
// 	~buffer() {}

// 	data()

// private:
// 	T * const base_;
// 	const size_t size_;
// };

extern API io::sink_adapter<uint8_t, std::ostream> cout;
extern API io::sink_adapter<uint8_t, std::ostream> cerr;

} // namespace nx::io

#endif