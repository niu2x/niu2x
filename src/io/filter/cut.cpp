#include "../io.h"

#include <string.h>

namespace nx::io::filter {

cut::cut(uint8_t chr)
: chr_(chr)
{
}

void cut::transform(ringbuf<uint8_t>& rbuf, ringbuf<uint8_t>& wbuf)
{
    uint8_t c;
    while ((!rbuf.empty()) && (!wbuf.full())) {
        rbuf.get(c);
        if (c != chr_)
            wbuf.put(c);
    }
}

} // namespace nx::io::filter
