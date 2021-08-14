#ifndef NX_MISC_NONCOPYABLE_H
#define NX_MISC_NONCOPYABLE_H

namespace nx::misc {

// noncopyable
class noncopyable {
public:
    noncopyable() = default;
    ~noncopyable() = default;

    noncopyable(const noncopyable&) = delete;
    noncopyable& operator=(const noncopyable&) = delete;
};

} // namespace nx::misc

#endif