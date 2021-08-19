#ifndef NX_UTILS_EXCEPTION_H
#define NX_UTILS_EXCEPTION_H

#include <exception>
#include <string>

#include <niu2x/api.h>
#include <niu2x/utils/limits.h>

#if defined(_WIN32) || defined(_WIN64)
    #pragma warning(disable : 4275)
#endif

namespace nx {

// exception
class API exception : public std::exception {
public:
    exception(const char* msg, const char* file, uint32_t line)
    {
        const char* fmt = "%s\nat: %s line: %d";
        snprintf(msg_, buffer_size, fmt, msg, file, line);
    }

    exception(const std::string& sz, const char* file, uint32_t line)
    : exception(sz.c_str(), file, line)
    {
    }

    virtual ~exception() { }
    virtual const char* what() const noexcept override { return msg_; }

private:
    enum { buffer_size = limits::max_error_message };
    char msg_[buffer_size];
};

} // namespace nx

#define NX_THROW(message) throw nx::exception((message), __FILE__, __LINE__)

#endif