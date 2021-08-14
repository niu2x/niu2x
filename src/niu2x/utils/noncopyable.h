#ifndef NX_UTILS_NONCOPYABLE_H
#define NX_UTILS_NONCOPYABLE_H

#include <niu2x/api.h>

namespace nx {

// noncopyable
class API noncopyable {
public:
    noncopyable() = default;
    ~noncopyable() = default;

    noncopyable(const noncopyable&) = delete;
    noncopyable& operator=(const noncopyable&) = delete;
};

} // namespace nx

#endif