#include <niu2x/io.h>

#include "pipe.h"

namespace nxlua {

std::string pipe(const std::string& src, int p_filter)
{
    nx::io::inbuf source(src.c_str(), src.length());

    std::string result;
    nx::io::string sink(result);

    nx::io::source_proxy source_proxy(&source);
    nx::io::sink_proxy sink_proxy(&sink);

    switch (p_filter) {
        case filter::hex: {
            nx::io::pipe(source_proxy, nx::io::hex, sink_proxy);
            break;
        }
        case filter::unhex: {
            nx::io::pipe(source_proxy, nx::io::unhex, sink_proxy);
            break;
        }
        case filter::zlib: {
            nx::io::zlib_filter zlib_filter;
            nx::io::pipe(
                source_proxy, nx::io::filter_proxy(&zlib_filter), sink_proxy);
            break;
        }
        case filter::unzlib: {
            nx::io::unzlib_filter unzlib_filter;
            nx::io::pipe(
                source_proxy, nx::io::filter_proxy(&unzlib_filter), sink_proxy);
            break;
        }
    }

    return result;
}

} // namespace nxlua