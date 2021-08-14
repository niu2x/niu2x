#ifndef NX_UTILS_UNUSED_H
#define NX_UTILS_UNUSED_H

#include <memory>

namespace nx {

// unused
inline void unused() { }

template <class FirstParam, class... Params>
inline void unused(FirstParam&& first_param, Params&&... params)
{
    (void)first_param;
    unused(std::forward<Params>(params)...);
}

} // namespace nx

#endif