#ifndef NX_IO_H
#define NX_IO_H

#include <niu2x/misc/ringbuffer.h>
#include <niu2x/misc/rw_status.h>
#include <niu2x/utils.h>

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
class filter {
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
    filtered_source(source<IE>& src, const filter<IE, OE>& p_filter)
    : src_(src)
    , filter_(p_filter)
    {
    }
    virtual ~filtered_source(){}

    virtual status get(OE* output, size_t max, size_t* osize) override
    {

        size_t readn = 0;
        if (obuf_.get(output, max, &readn) == status::ok) {
            if (osize)
                *osize = readn;
            return status::ok;
        }

        if (ibuf_.size()) {
            IE tmp_input[CHUNK];
            IE tmp_output[CHUNK];
            ibuf_.get(tmp_input, CHUNK, nullptr);
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
    filter<IE, OE> filter_;

    misc::ringbuffer<IE, CHUNK> ibuf_;
    misc::ringbuffer<IE, CHUNK> obuf_;
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
        delegate_.write(reinterpret_cast<const char*>(output), isize / 2 + 1);
        if (osize)
            *osize = isize / 2 + 1;
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

// template<class IE, class OE>
// void pipe(source<IE> &src, const filter<IE, OE>& flt, sink<OE> &dst, size_t
// chunk = 4_k) {
//     std::vector<IE> ibuf;
//     ibuf.resize(chunk);

//     std::vector<OE> obuf;
//     obuf.resize(chunk);

//     size_t ibuf_size = 0;
//     size_t obuf_size = 0;

//     size_t readn = 0;

//     while(true){

//         status s;
//         while( (s=src.get(ibuf.data(), chunk, &readn)) == status::again);
//         if(s == status::eof) break;

//         while(readn){
//             size_t icvtn, ocvtn;
//             flt.cvt(ibuf.data(), readn, &cvtn, obuf.data(), chunk, &ocvtn);
//         }

//         auto *ptr = buffer.data();
//         while(readn){
//             size_t writen = 0;
//             while((s=dst.put(ptr, readn, &writen)) == status::again);
//             if(s == status::eof) break;
//             readn -= writen;
//             ptr += writen;
//         }
//     }
// }
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