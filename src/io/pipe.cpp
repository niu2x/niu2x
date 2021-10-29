#include <niu2x/io.h>
#include <niu2x/ringbuf.h>
#include "utils.h"

namespace nx::io {

API bool operator|(source p_source, sink p_sink)
{
    ringbuf<unsigned char> buf;

    bool source_eof = false;

    do {
        while ((!buf.full()) && (!source_eof)) {
            auto slots = buf.continuous_slots();
            auto ret = p_source.read(slots.base, slots.size);

            if (ret >= 0) {
                buf.update_size(slots.size - ret);
            } else if (ret == -eof) {
                source_eof = true;
            } else if (ret == -again) {

            } else
                return false;
        }

        while (!buf.empty()) {
            auto elems = buf.continuous_elems();
            auto ret = p_sink.write(elems.base, elems.size);

            if (ret >= 0) {
                buf.update_size(-(elems.size - ret));
            } else if (ret == -again) {

            } else
                return false;
        }

    } while ((!source_eof) || (!buf.empty()));

    return true;
}

API bool operator|(filter::filter::proxy_t p_source, sink p_sink)
{
    ringbuf<unsigned char> buf;
    bool source_eof = false;
    do {
        while ((!buf.full()) && (!source_eof)) {
            auto slots = buf.continuous_slots();
            auto ret = p_source.read(slots.base, slots.size);

            if (ret >= 0) {
                buf.update_size(slots.size - ret);
            } else if (ret == -eof) {
                source_eof = true;
            } else if (ret == -again) {

            } else
                return false;
        }

        while (!buf.empty()) {
            auto elems = buf.continuous_elems();
            auto ret = p_sink.write(elems.base, elems.size);

            if (ret >= 0) {
                buf.update_size(-(elems.size - ret));
            } else if (ret == -again) {

            } else
                return false;
        }

    } while ((!source_eof) || (!buf.empty()));

    return true;
}

API filter::filter::proxy_t operator|(
    source p_source, filter::filter::proxy_t p_filter)
{
    p_filter.set_upstream(p_source);
    return p_filter;
}

} // namespace nx::io
