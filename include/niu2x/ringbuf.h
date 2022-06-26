#ifndef NX_RINGBUF_T
#define NX_RINGBUF_T

#include <boost/circular_buffer.hpp>

namespace nx {
template <class T>
using ringbuf_t = boost::circular_buffer<T>;
}

#endif
