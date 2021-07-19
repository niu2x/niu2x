#ifndef NX_IO_H
#define NX_IO_H

#include <niu2x/utils.h>

namespace nx::io {

void API read_file(const char *pathname, std::vector<uint8_t> &output);

enum class state {
    on,
    off,
};

enum class status {
    ok,
    again,
    eof,
};

template <class Elem> class API source : private noncopyable {
public:
    source() { }
    virtual ~source() { }
    status get_elem(Elem& elem) { return get(1, &elem, nullptr); }
    virtual status get(size_t max, Elem* output, size_t* osize) = 0;
};

template <class Elem> class API sink : private noncopyable {
public:
    sink() { }
    virtual ~sink() { }
    virtual status put(const Elem& elem) = 0;
};

template <class Elem> class API filter : private noncopyable {
public:
};

template <class Elem> class API bufsource : public source<Elem> {
public:
    bufsource(const Elem* const base, size_t size)
    : base_(base)
    , size_(size)
    , read_index_(0)
    {
    }
    virtual ~bufsource() { }

    virtual status get(size_t max, Elem* output, size_t* osize) override
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

} // namespace nx::io

#endif