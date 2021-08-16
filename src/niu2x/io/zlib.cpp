#include <niu2x/io.h>

namespace nx::io {

static constexpr int default_level = 6;

static void zlib_setup(z_stream& strm, int level)
{
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    auto ret = deflateInit(&strm, level);
    NX_ASSERT(ret == Z_OK, "zlib compress deflateInit failed");
}

static void zlib_cleanup(z_stream& strm) { deflateEnd(&strm); }

zlib_filter::zlib_filter(int level)
: level_(level == -1 ? default_level : level)
{
    zlib_setup(strm_, level_);
}

zlib_filter::~zlib_filter() { zlib_cleanup(strm_); }

void zlib_filter::reset()
{
    zlib_cleanup(strm_);
    zlib_setup(strm_, level_);
}

static void unzlib_setup(z_stream& strm)
{
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    auto ret = inflateInit(&strm);
    NX_ASSERT(ret == Z_OK, "zlib compress inflateInit failed");
}

static void unzlib_cleanup(z_stream& strm) { inflateEnd(&strm); }

unzlib_filter::unzlib_filter() { unzlib_setup(strm_); }

unzlib_filter::~unzlib_filter() { unzlib_cleanup(strm_); }

void unzlib_filter::reset()
{
    unzlib_cleanup(strm_);
    unzlib_setup(strm_);
}

#define DEF_CVT(name, func)                                                    \
    void name##_filter::cvt(const uint8_t* in, size_t isize, uint8_t* out,     \
        size_t max_osize, size_t& readn, size_t& writen)                       \
    {                                                                          \
        strm_.avail_in = static_cast<uInt>(isize);                             \
        int flush = isize ? Z_NO_FLUSH : Z_FINISH;                             \
        strm_.next_in = const_cast<Bytef*>(in);                                \
        strm_.avail_out = static_cast<uInt>(max_osize);                        \
        strm_.next_out = out;                                                  \
        auto ret = func(&strm_, flush);                                        \
        NX_ASSERT(ret == Z_OK || ret == Z_STREAM_END, #name " failed");        \
        auto have = max_osize - strm_.avail_out;                               \
        writen = have;                                                         \
        readn = isize - strm_.avail_in;                                        \
    }

DEF_CVT(unzlib, inflate)
DEF_CVT(zlib, deflate)

} // namespace nx::io