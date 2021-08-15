#include <niu2x/io.h>

namespace nx::io {

static constexpr int default_level = 6;

static void zlib_compress_setup(z_stream& strm, int level)
{
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    auto ret = deflateInit(&strm, level);
    NX_ASSERT(ret == Z_OK, "zlib compress deflateInit failed");
}

static void zlib_compress_cleanup(z_stream& strm) { deflateEnd(&strm); }

zlib_compress_filter::zlib_compress_filter(int level)
: level_(level == -1 ? default_level : level)
{
    zlib_compress_setup(strm_, level_);
}

zlib_compress_filter::~zlib_compress_filter() { zlib_compress_cleanup(strm_); }

void zlib_compress_filter::reset()
{
    zlib_compress_cleanup(strm_);
    zlib_compress_setup(strm_, level_);
}

static void zlib_uncompress_setup(z_stream& strm)
{
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    auto ret = inflateInit(&strm);
    NX_ASSERT(ret == Z_OK, "zlib compress inflateInit failed");
}

static void zlib_uncompress_cleanup(z_stream& strm) { inflateEnd(&strm); }

zlib_uncompress_filter::zlib_uncompress_filter()
{
    zlib_uncompress_setup(strm_);
}

zlib_uncompress_filter::~zlib_uncompress_filter()
{
    zlib_uncompress_cleanup(strm_);
}

void zlib_uncompress_filter::reset()
{
    zlib_uncompress_cleanup(strm_);
    zlib_uncompress_setup(strm_);
}

#define DEF_CVT(name, func)                                                    \
    void zlib_##name##_filter::cvt(const uint8_t* in, size_t isize,            \
        uint8_t* out, size_t max_osize, size_t& readn, size_t& writen)         \
    {                                                                          \
        strm_.avail_in = static_cast<uInt>(isize);                             \
        int flush = isize ? Z_NO_FLUSH : Z_FINISH;                             \
        strm_.next_in = const_cast<Bytef*>(in);                                \
        strm_.avail_out = static_cast<uInt>(max_osize);                        \
        strm_.next_out = out;                                                  \
        auto ret = func(&strm_, flush);                                        \
        NX_ASSERT(                                                             \
            ret == Z_OK || ret == Z_STREAM_END, "zlib " #name " failed");      \
        auto have = max_osize - strm_.avail_out;                               \
        writen = have;                                                         \
        readn = isize - strm_.avail_in;                                        \
    }

DEF_CVT(uncompress, inflate)
DEF_CVT(compress, deflate)

} // namespace nx::io