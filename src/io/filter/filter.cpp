#include "../io.h"
#include <niu2x/log.h>
#include <string.h>

namespace nx::io::filter {

filter::filter()
: upstream_(nullptr)
, upstream_eof_(false)
{
}

filter::~filter() { }

int filter::read(void* data, size_t bytes)
{
    if (upstream_eof_ && rbuf_.empty() && wbuf_.empty())
        return -eof;

    read_from_upstream();
    rbuf_.normalize();
    wbuf_.normalize();
    transform(rbuf_, wbuf_, upstream_eof_);
    return write_to_downstream(data, bytes);
}

void filter::set_upstream(source p_upstream)
{
    upstream_ = std::move(p_upstream);
}

void filter::set_upstream(proxy_t p_upstream)
{
    upstream_ = std::move(p_upstream);
}

void filter::read_from_upstream()
{
    while (!rbuf_.full() && !upstream_eof_) {
        auto slots = rbuf_.continuous_slots();

        auto ret = std::visit(
            [&slots](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, nullptr_t>) {
                    return -eof;
                } else {
                    return arg.read(slots.base, slots.size);
                }
            },
            upstream_);

        if (ret >= 0) {
            rbuf_.update_size(slots.size - ret);
        } else if (ret == -eof) {
            upstream_eof_ = true;
        } else if (ret == -again) {

        } else
            return;
    }
}

int filter::write_to_downstream(void* p_data, size_t bytes)
{
    auto* data = reinterpret_cast<uint8_t*>(p_data);

    auto elems = wbuf_.continuous_elems();
    if (elems.size > 0 && bytes > 0) {
        auto writen = std::min(elems.size, bytes);
        memcpy(data, elems.base, writen);
        wbuf_.update_size(-writen);
        data += writen;
        bytes -= writen;
    }

    elems = wbuf_.continuous_elems();
    if (elems.size > 0 && bytes > 0) {
        auto writen = std::min(elems.size, bytes);
        memcpy(data, elems.base, writen);
        wbuf_.update_size(-writen);
        bytes -= writen;
    }

    return bytes;
}

API filter::proxy_t operator|(
    filter::proxy_t p_source, filter::proxy_t p_filter)
{
    p_filter.set_upstream(p_source);
    return p_filter;
}

} // namespace nx::io::filter
