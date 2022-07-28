#ifndef NX_UTILS_H
#define NX_UTILS_H

#include <functional>
#include <string>
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
    ~func_atexit_t() { delegate_(); }

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

#define NX_FAIL_COND(cond)                                                     \
    if (cond) {                                                                \
        return;                                                                \
    }

#define NX_FAIL_COND_MSG(cond, ...)                                            \
    if (cond) {                                                                \
        NX_LOG_E(__VA_ARGS__);                                                 \
        return;                                                                \
    }
#define NX_FAIL_COND_V_MSG(cond, result, ...)                                  \
    if (cond) {                                                                \
        NX_LOG_E(__VA_ARGS__);                                                 \
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

// NX_OFFSET_OF
#define NX_OFFSET_OF(clazz, field) ((size_t) & (((clazz*)(nullptr))->field))
// NX_OFFSET_OF

#define NX_ADDRESS_OFFSET(type, base, offset)                                  \
    (type)((uint8_t*)((base)) + (offset))

#define NX_CONTAINER_OF(ptr, clazz, field)                                     \
    ((clazz*)(((uint8_t*)(ptr)) - NX_OFFSET_OF(clazz, field)))

#if !defined(NX_LOG_E)
    #define NX_LOG_E(...)                                                      \
        {                                                                      \
            fprintf(stderr, __VA_ARGS__);                                      \
            fprintf(stderr, "\n");                                             \
        }
#endif

#define NX_ROUND_MASK(x, y) ((__typeof__(x))((y)-1))
#define NX_ROUND_UP(x, y)   ((((x)-1) | NX_ROUND_MASK(x, y)) + 1)
#define NX_ROUND_DOWN(x, y) ((x) & ~NX_ROUND_MASK(x, y))

#define NX_U64(n) ((uint64_t)(n))
// #define NX_U32(n) ((uint32_t)(n))

#define NX_BITS_PER_BYTE       8
#define NX_BITS_PER_TYPE(type) (sizeof(type) * NX_BITS_PER_BYTE)
// #define NX_BITS_PER_U32        NX_BITS_PER_TYPE(uint32_t)
#define NX_BITS_PER_U64        NX_BITS_PER_TYPE(uint64_t)

#define NX_BIT_U64(nr) (NX_U64(1) << (nr))

#define NX_BIT_U64_MASK(nr) (NX_U64(1) << ((nr) % NX_BITS_PER_U64))
#define NX_BIT_U64_WORD(nr) ((nr) / NX_BITS_PER_U64)

#define NX_DIV_ROUND_UP(n, d) (((n) + (d)-1) / (d))

#define NX_BITS_TO_U64(nr)   NX_DIV_ROUND_UP(nr, NX_BITS_PER_TYPE(uint64_t))
// #define NX_BITS_TO_U32(nr)   NX_DIV_ROUND_UP(nr, NX_BITS_PER_TYPE(uint32_t))
#define NX_BITS_TO_BYTES(nr) NX_DIV_ROUND_UP(nr, NX_BITS_PER_TYPE(char))

namespace nx {

template <class T>
void call_destructor(T* obj)
{
    obj->~T();
}

} // namespace nx

#endif