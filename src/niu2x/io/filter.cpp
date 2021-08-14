#include <niu2x/io.h>

#include <algorithm>

namespace nx::io {

filter::filter()
: next_sink_(null)
, next_type_(nt::none)
{
}

filter::~filter() { }

template <class OUT>
static status tput(filter* self, const uint8_t* output, size_t isize,
    size_t& osize, ringbuffer& buf, OUT& p_out)
{
    if (isize) {
        osize = 0;
        while (isize) {
            auto elems = buf.continuous_elems();
            if (elems.size) {
                size_t writen;
                int status = p_out.put(elems.base, elems.size, writen);
                if (status == ok) {
                    buf.pop(writen);
                }
            }

            auto slots = buf.continuous_slots();
            if (slots.size) {
                size_t readn, writen;
                self->cvt(output, isize, slots.base, slots.size, readn, writen);
                buf.push(writen);
                osize += readn;
                isize -= readn;
                output += readn;
            } else {
                break;
            }

            elems = buf.continuous_elems();
            if (elems.size) {
                size_t writen;
                int status = p_out.put(elems.base, elems.size, writen);
                if (status == ok) {
                    buf.pop(writen);
                }
            }
        }
        return osize ? ok : again;
    } else {
        while (true) {

            auto elems = buf.continuous_elems();
            if (elems.size) {
                size_t writen;
                int status = p_out.put(elems.base, elems.size, writen);
                if (status == ok) {
                    buf.pop(writen);
                }
            }

            auto slots = buf.continuous_slots();
            if (slots.size) {
                size_t readn, writen;
                self->cvt(nullptr, 0, slots.base, slots.size, readn, writen);
                buf.push(writen);
            }

            if (buf.empty())
                break;

            elems = buf.continuous_elems();
            if (elems.size) {
                size_t writen;
                int status = p_out.put(elems.base, elems.size, writen);
                if (status == ok) {
                    buf.pop(writen);
                } else {
                    break;
                }
            }
        }

        return buf.empty() ? ok : again;
    }
}

status filter::put(const uint8_t* output, size_t isize, size_t& osize)
{
    NX_ASSERT(next_type_ != nt::none, "no next");

    if (next_type_ == nt::sink) {
        return tput<sink_proxy>(this, output, isize, osize, buf_, next_sink_);
    } else {
        return tput<filter>(this, output, isize, osize, buf_, *next_filter_);
    }
}

void filter::set_next(sink_proxy next)
{
    next_sink_ = next;
    next_type_ = nt::sink;
}

void filter::set_next(filter* next)
{
    next_filter_ = next;
    next_type_ = nt::filter;
}

simple_filter::simple_filter(const converter& p_converter)
: converter_(p_converter)
{
}

simple_filter::~simple_filter() { }

void simple_filter::reset() { }

void simple_filter::cvt(const uint8_t* in, size_t isize, uint8_t* out,
    size_t max_osize, size_t& readn, size_t& writen)
{
    readn = writen = NX_MIN(isize, max_osize);
    std::transform(in, in + readn, out, converter_);
}

static uint8_t hex(uint8_t digit)
{
    if (digit < 10)
        return '0' + digit;
    return 'a' + digit - 10;
}

static uint8_t unhex(uint8_t digit)
{
    if (digit <= '9')
        return digit - '0';

    if (digit <= 'F')
        return digit - 'A' + 10;

    return digit - 'a' + 10;
}

hex_encode_filter::hex_encode_filter() { }

hex_encode_filter::~hex_encode_filter() { }

void hex_encode_filter::reset() { }

void hex_encode_filter::cvt(const uint8_t* in, size_t isize, uint8_t* out,
    size_t max_osize, size_t& readn, size_t& writen)
{
    readn = NX_MIN(isize, max_osize >> 1);
    writen = readn << 1;

    for (size_t i = 0; i < readn; i++) {
        uint8_t c = *in++;
        *out++ = hex(c >> 4);
        *out++ = hex(c & 0xF);
    }
}

hex_decode_filter::hex_decode_filter() { }

hex_decode_filter::~hex_decode_filter() { }

void hex_decode_filter::reset() { }

void hex_decode_filter::cvt(const uint8_t* in, size_t isize, uint8_t* out,
    size_t max_osize, size_t& readn, size_t& writen)
{
    writen = NX_MIN(isize >> 1, max_osize);
    readn = writen << 1;

    for (size_t i = 0; i < writen; i++) {
        *out++ = (unhex(*in) << 4) | unhex(*(in + 1));
        in += 2;
    }
}

static simple_filter filter_one([](uint8_t c) { return c; });
static simple_filter filter_lower([](uint8_t c) { return tolower(c); });
static simple_filter filter_upper([](uint8_t c) { return toupper(c); });
static hex_encode_filter my_hex_encode_filter;
static hex_decode_filter my_hex_decode_filter;

filter_proxy one(&filter_one);
filter_proxy lower(&filter_lower);
filter_proxy upper(&filter_upper);
filter_proxy hex_encode(&my_hex_encode_filter);
filter_proxy hex_decode(&my_hex_decode_filter);

} // namespace nx::io