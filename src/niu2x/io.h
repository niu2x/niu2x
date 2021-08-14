#ifndef NX_IO_H
#define NX_IO_H

#include <iostream>

#include <niu2x/utils.h>

namespace nx::io {

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
    source_proxy(source& delegate)
    : delegate_(delegate)
    {
    }
    ~source_proxy() { }
    source_proxy(const source_proxy&) = default;
    source_proxy& operator=(const source_proxy&) = default;
    status get(uint8_t* output, size_t max, size_t& osize)
    {
        return delegate_.get(output, max, osize);
    }
    status get_elem(uint8_t& elem) { return delegate_.get_elem(elem); }

private:
    source& delegate_;
};

class API sink_proxy {
public:
    sink_proxy(sink& delegate)
    : delegate_(delegate)
    {
    }
    ~sink_proxy() { }
    sink_proxy(const sink_proxy&) = default;
    sink_proxy& operator=(const sink_proxy&) = default;
    status put(const uint8_t* output, size_t isize, size_t& osize)
    {
        return delegate_.put(output, isize, osize);
    }
    status put_elem(uint8_t elem) { return delegate_.put_elem(elem); }

private:
    sink& delegate_;
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

extern API sink_proxy cout;
extern API sink_proxy cerr;
extern API source_proxy cin;

API void pipe(source_proxy& src, sink_proxy& dst);

} // namespace nx::io

#endif