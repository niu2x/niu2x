#ifndef NX_IO_H
#define NX_IO_H

#include <iostream>
#include <functional>
#include <variant>
#include <string>

#include <boost/noncopyable.hpp>

#include <niu2x/api.h>
#include <niu2x/ringbuf.h>

namespace nx::io {

// std::string a = "hello";
// std::string b;

// nx::io::source(a) | nx::io::filter::upper | nx::io::sink(b);
// nx::io::source(std::cin) | nx::io::filter::upper | nx::io::sink(std::cout);
// nx::io::source(std::cin) | nx::io::filter::b64enc | nx::io::sink(std::cout);
// nx::io::source(std::cin) | nx::io::filter::b64dec | nx::io::sink(std::cout);

// class source_adapter {
// public:
// };
//
//
enum API status {
    ok = 0,
    again,
    eof,
    fail,
};

class API source {
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

class API sink {
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

class API filter {
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

    filter(const filter&) = default;
    filter& operator=(const filter&) = default;

    int read(void* data, size_t bytes);

    void set_upstream(source p_upstream);
    void set_upstream(proxy_t p_upstream);

    virtual void transform(ringbuf<uint8_t>&, ringbuf<uint8_t>&) = 0;

    proxy_t proxy() { return proxy_t(this); }

private:
    void read_from_upstream();
    int write_to_downstream(void* data, size_t bytes);

    std::variant<source, proxy_t, std::nullptr_t> upstream_;
    ringbuf<uint8_t> rbuf_;
    ringbuf<uint8_t> wbuf_;
    bool upstream_eof_;
};

class API simple_filter : public filter {
public:
    virtual void transform(ringbuf<uint8_t>&, ringbuf<uint8_t>&);
    virtual uint8_t transform(uint8_t chr) = 0;
};

class API lower : public simple_filter {
public:
    virtual uint8_t transform(uint8_t chr) { return tolower(chr); }
};

}; // namespace filter

API bool operator|(source p_source, sink p_sink);
API bool operator|(filter::filter::proxy_t p_filter, sink p_sink);
API filter::filter::proxy_t operator|(
    source p_source, filter::filter::proxy_t p_filter);

} // namespace nx::io

#endif