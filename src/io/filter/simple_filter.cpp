#include "../io.h"

#include <string.h>

namespace nx::io::filter {

bool simple_filter::transform(ringbuf& rbuf, ringbuf& wbuf, bool upstream_eof)
{
    (void)upstream_eof;
    uint8_t chr;
    while (!rbuf.empty() && !wbuf.full()) {
        rbuf.get(chr);
        wbuf.put(transform(chr));
    }
    return true;
}

} // namespace nx::io::filter
