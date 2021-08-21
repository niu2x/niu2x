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
        case pipe_filter::hex: {
            nx::io::pipe(source_proxy, nx::io::hex, sink_proxy);
            break;
        }
        case pipe_filter::unhex: {
            nx::io::pipe(source_proxy, nx::io::unhex, sink_proxy);
            break;
        }
        case pipe_filter::zlib: {
            nx::io::zlib_filter zlib_filter;
            nx::io::pipe(
                source_proxy, nx::io::filter_proxy(&zlib_filter), sink_proxy);
            break;
        }
        case pipe_filter::unzlib: {
            nx::io::unzlib_filter unzlib_filter;
            nx::io::pipe(
                source_proxy, nx::io::filter_proxy(&unzlib_filter), sink_proxy);
            break;
        }
    }

    return result;
}

std::string pipe_digest(const std::string& src, const char* algorithm)
{
    nx::io::inbuf source(src.c_str(), src.length());

    std::string result;
    nx::io::string sink(result);

    nx::io::source_proxy source_proxy(&source);
    nx::io::sink_proxy sink_proxy(&sink);

    nx::io::digest my_digest(algorithm);

    nx::io::pipe(source_proxy, nx::io::filter_proxy(&my_digest), sink_proxy);

    return result;
}

std::string pipe_cipher(const std::string& src, int mode, const char* algorithm,
    const std::string& key, const std::string& iv)
{
    nx::io::inbuf source(src.c_str(), src.length());

    std::string result;
    nx::io::string sink(result);

    nx::io::source_proxy source_proxy(&source);
    nx::io::sink_proxy sink_proxy(&sink);

    nx::io::cipher filter(mode, algorithm,
        reinterpret_cast<const uint8_t*>(key.c_str()),
        reinterpret_cast<const uint8_t*>(iv.c_str()));

    nx::io::pipe(source_proxy, nx::io::filter_proxy(&filter), sink_proxy);

    return result;
}

} // namespace nxlua