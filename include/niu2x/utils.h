#ifndef NX_UTILS_H
#define NX_UTILS_H

#include <functional>
#include <boost/noncopyable.hpp>
#include <boost/preprocessor.hpp>

#include <niu2x/api.h>
#include <niu2x/errcode.h>

// func atexit
namespace nx {

class func_atexit_t : private boost::noncopyable {
public:
    template <class T>
    func_atexit_t(T&& d)
    {
        delegate_ = std::forward<T>(d);
    }
    ~func_atexit_t() { }

private:
    std::function<void()> delegate_;
};

} // namespace nx

#define NX_FUNC_AT_EXIT(name, delegate) func_atexit_t name((delegate));
// func atexit

// fail
namespace nx {

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

#define NX_FAIL_COND_V(cond, result)                                           \
    if (cond) {                                                                \
        return result;                                                         \
    }

#define NX_THROW_COND_MSG(cond, msg)                                           \
    if (cond) {                                                                \
        throw nx::exception((msg), __FILE__, __LINE__);                        \
    }
// fail

// unused
namespace nx {

inline void unused(...) { }

} // namespace nx

// unused
#define NX_UNUSED_ONE(r, data, elem) (void)(elem);

#define NX_UNUSED(...)                                                         \
    BOOST_PP_SEQ_FOR_EACH(                                                     \
        NX_UNUSED_ONE, ~, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__));
// unused

#endif