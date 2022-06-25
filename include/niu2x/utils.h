#ifndef NX_UTILS_H
#define NX_UTILS_H

#include <string>
#include <exception>
#include <boost/preprocessor.hpp>

namespace nx {

inline void unused(...) { }

// exception
class exception : public std::exception {
public:
    exception(const char* msg, const char* file, uint32_t line)
    {
        snprintf(msg_, buffer_size, "%s\nat: %s line: %d", msg, file, line);
    }

    exception(const std::string& sz, const char* file, uint32_t line)
    : exception(sz.c_str(), file, line)
    {
    }

    virtual ~exception() { }
    virtual const char* what() const noexcept override { return msg_; }

private:
    // enum { buffer_size = limits::max_error_message };
    enum { buffer_size = 1024 };
    char msg_[buffer_size];
};

} // namespace nx

#define NX_UNUSED_ONE(r, data, elem) (void)(elem);

#define NX_UNUSED(...)                                                         \
    BOOST_PP_SEQ_FOR_EACH(                                                     \
        NX_UNUSED_ONE, ~, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__));

#define NX_FAIL_COND_V(cond, result)                                           \
    if (cond) {                                                                \
        return result;                                                         \
    }

#define NX_THROW_COND_MSG(cond, msg)                                           \
    if (cond) {                                                                \
        throw nx::exception((msg), __FILE__, __LINE__);                        \
    }

#endif