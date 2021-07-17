#ifndef NIU2X_UTILS_H
#define NIU2X_UTILS_H

#include <cstdio>
#include <utility>
#include <cstdint>
#include <cstring>

#include <boost/noncopyable.hpp>

#include <niu2x/api.h>

inline void unused() {}

template <class FirstParam, class... Params>
inline void unused(FirstParam &&first_param, Params &&... params) {
    (void)first_param;
    unused(std::forward<Params>(params)...);
}

using noncopyable = boost::noncopyable;

#endif