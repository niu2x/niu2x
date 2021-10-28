#ifndef NX_IO_H
#define NX_IO_H

#include <iostream>
#include <functional>
#include <variant>
#include <string>

#include <boost/noncopyable.hpp>

#include <niu2x/api.h>

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
    source& operator=(const source&) = default;

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

API bool operator|(source p_source, sink p_sink);

} // namespace nx::io

#endif