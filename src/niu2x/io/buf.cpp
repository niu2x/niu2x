#include <niu2x/io.h>

#include <string.h>

namespace nx::io {

inbuf::inbuf(const void* base, size_t size)
: buffer_ { base, size }
, read_pos_(0)
{
}

inbuf::~inbuf() { }

status inbuf::get(uint8_t* output, size_t max, size_t& osize)
{
    if (buffer_.size == read_pos_)
        return eof;

    osize = NX_MIN(buffer_.size - (read_pos_), max);

    auto* input = reinterpret_cast<const uint8_t*>(buffer_.base) + read_pos_;
    memcpy(output, input, osize);
    read_pos_ += osize;
    return ok;
}

vector::vector(std::vector<uint8_t>& backend)
: backend_(backend)
{
}

vector::~vector() { }

status vector::put(const uint8_t* output, size_t max, size_t& osize)
{
    backend_.insert(backend_.end(), output, output + max);
    osize = max;
    return status::ok;
}

} // namespace nx::io