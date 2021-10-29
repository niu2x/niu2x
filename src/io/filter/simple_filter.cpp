#include "../io.h"

#include <string.h>

namespace nx::io::filter {

void simple_filter::transform(ringbuf<uint8_t>& rbuf, ringbuf<uint8_t>& wbuf)
{
    uint8_t chr;
    while ((!rbuf.empty()) && (!wbuf.full())) {
        rbuf.get(chr);
        wbuf.put(transform(chr));
    }
}

} // namespace nx::io::filter
