#include <niu2x/io.h>

#include <zlib.h>

namespace nx::io::filter {

struct zlib_compress_t::context {
    z_stream strm;
    std::vector<uint8_t> buffer;
};

status zlib_compress_t::cvt(const uint8_t* input, size_t isize,
    size_t* consumed_isize, uint8_t* output, size_t max_osize, size_t* osize)
{
    NX_ASSERT(isize <= std::numeric_limits<uInt>::max(), "zlib_compress_t cvt fail: too large isize");
    NX_ASSERT(max_osize <= std::numeric_limits<uInt>::max(), "zlib_compress_t cvt fail: too large max_osize");
    if (!ctx_) {
        ctx_ = std::make_unique<context>();
        ctx_->strm.zalloc = Z_NULL;
        ctx_->strm.zfree = Z_NULL;
        ctx_->strm.opaque = Z_NULL;

        auto ret = deflateInit(&(ctx_->strm), level_);
        NX_ASSERT(ret == Z_OK, "zlib compress deflateInit failed");
    }

    auto& strm = ctx_->strm;

    strm.avail_in = static_cast<uInt>(isize);
    int flush = isize ? Z_NO_FLUSH : Z_FINISH;
    strm.next_in = const_cast<Bytef*>(input);

    strm.avail_out = static_cast<uInt>(max_osize);
    strm.next_out = output;

    auto ret = deflate(&strm, flush);
    NX_ASSERT(
        ret == Z_OK || ret == Z_STREAM_END, "zlib compress deflate failed");
    auto have = max_osize - strm.avail_out;

    if (osize)
        *osize = have;
    if (consumed_isize)
        *consumed_isize = isize - strm.avail_in;

    if (isize) {
        return have ? status::ok
                    : (isize - strm.avail_in
                            ? status::ok
                            : ((max_osize >= 4 ? status::eof : status::again)));
    } else {
        return have ? status::ok
                    : (max_osize >= 4 ? status::eof : status::again);
    }
}

zlib_compress_t::zlib_compress_t(const zlib_compress_t& other)
: ctx_(nullptr)
, level_(other.level_)
{
}

zlib_compress_t::zlib_compress_t(int level)
: ctx_(nullptr)
{
    level_ = level == -1 ? 6 : level_;
    NX_ASSERT(level_ >= 0 && level_ <= 9, "zlib_compress invalid level");
}

zlib_compress_t::~zlib_compress_t()
{
    if (ctx_)
        deflateEnd(&(ctx_->strm));
}


struct zlib_uncompress_t::context {
    z_stream strm;
    std::vector<uint8_t> buffer;
};

status zlib_uncompress_t::cvt(const uint8_t* input, size_t isize,
    size_t* consumed_isize, uint8_t* output, size_t max_osize, size_t* osize)
{   
    NX_ASSERT(isize <= std::numeric_limits<uInt>::max(), "zlib_compress_t cvt fail: too large isize");
    NX_ASSERT(max_osize <= std::numeric_limits<uInt>::max(), "zlib_compress_t cvt fail: too large max_osize");

    if (!ctx_) {
        ctx_ = std::make_unique<context>();
        ctx_->strm.zalloc = Z_NULL;
        ctx_->strm.zfree = Z_NULL;
        ctx_->strm.opaque = Z_NULL;

        auto ret = inflateInit(&(ctx_->strm));
        NX_ASSERT(ret == Z_OK, "zlib uncompress inflateInit failed");
    }

    auto& strm = ctx_->strm;

    strm.avail_in = static_cast<uInt>(isize);
    int flush = isize ? Z_NO_FLUSH : Z_FINISH;
    strm.next_in = const_cast<Bytef*>(input);

    strm.avail_out = static_cast<uInt>(max_osize);
    strm.next_out = output;

    auto ret = inflate(&strm, flush);
    NX_ASSERT(
        ret == Z_OK || ret == Z_STREAM_END, "zlib uncompress inflate failed");

    auto have = max_osize - strm.avail_out;

    if (osize)
        *osize = have;
    if (consumed_isize)
        *consumed_isize = isize - strm.avail_in;

    if (isize) {
        return have ? status::ok
                    : (isize - strm.avail_in
                            ? status::ok
                            : ((max_osize >= 4 ? status::eof : status::again)));
    } else {
        return have ? status::ok
                    : (max_osize >= 4 ? status::eof : status::again);
    }
}

zlib_uncompress_t::zlib_uncompress_t(const zlib_uncompress_t& other)
: ctx_(nullptr)
{
    unused(other);
}

zlib_uncompress_t::zlib_uncompress_t()
: ctx_(nullptr)
{
}

zlib_uncompress_t::~zlib_uncompress_t()
{
    if (ctx_)
        inflateEnd(&(ctx_->strm));
}

zlib_compress_t zlib_compress;
zlib_uncompress_t zlib_uncompress;

} // namespace nx::io::filter