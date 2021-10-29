#include "../io.h"

#include <string.h>

namespace nx::io::filter {

namespace {

static uint8_t hex_digit(uint8_t digit)
{
    if (digit < 10)
        return '0' + digit;
    return 'a' + digit - 10;
}

static uint8_t unhex_digit(uint8_t digit)
{
    if (digit <= '9')
        return digit - '0';

    if (digit <= 'F')
        return digit - 'A' + 10;

    return digit - 'a' + 10;
}

} // namespace

void hex::transform(ringbuf<uint8_t>& rbuf, ringbuf<uint8_t>& wbuf)
{
    uint8_t chr;
    while ((!rbuf.empty()) && (wbuf.user_capacity - wbuf.size() >= 2)) {
        rbuf.get(chr);
        wbuf.put(hex_digit(chr >> 4));
        wbuf.put(hex_digit(chr & 0xF));
    }
}

unhex::unhex()
: size_(0)
{
}

void unhex::transform(ringbuf<uint8_t>& rbuf, ringbuf<uint8_t>& wbuf)
{
    while ((!rbuf.empty()) && (!wbuf.full())) {

        while (size_ < 2 && (!rbuf.empty())) {
            rbuf.get(buf_[size_++]);
        }

        if (size_ < 2)
            break;

        wbuf.put((unhex_digit(buf_[0]) << 4) | unhex_digit(buf_[1]));
        size_ = 0;
    }
}

} // namespace nx::io::filter
