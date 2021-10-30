#include "../io.h"

#include <string.h>

#include <zlib.h>

#include "assert.h"

namespace nx::io::filter {

namespace {

void zlib_setup(z_stream* strm, int level);
void zlib_cleanup(z_stream* strm);

void unzlib_setup(z_stream* strm);
void unzlib_cleanup(z_stream* strm);

} // namespace

zlib::zlib(int level)
{
    zlib_ctx_ = mem.allocate(sizeof(z_stream));
    NX_ASSERT(zlib_ctx_, "out of memory");
    zlib_setup(reinterpret_cast<z_stream*>(zlib_ctx_), level);
}
zlib::~zlib()
{
    zlib_cleanup(reinterpret_cast<z_stream*>(zlib_ctx_));
    mem.free(zlib_ctx_);
}

unzlib::unzlib()
{
    zlib_ctx_ = mem.allocate(sizeof(z_stream));
    NX_ASSERT(zlib_ctx_, "out of memory");
    unzlib_setup(reinterpret_cast<z_stream*>(zlib_ctx_));
}

unzlib::~unzlib()
{
    unzlib_cleanup(reinterpret_cast<z_stream*>(zlib_ctx_));
    mem.free(zlib_ctx_);
}

bool zlib::transform(ringbuf& rbuf, ringbuf& wbuf, bool upstream_eof)
{
    auto* strm_ = reinterpret_cast<z_stream*>(zlib_ctx_);

    auto input = rbuf.continuous_elems();
    auto output = wbuf.continuous_slots();

    strm_->avail_in = static_cast<uInt>(input.size);
    int flush = (upstream_eof && rbuf.empty()) ? Z_FINISH : Z_NO_FLUSH;
    strm_->next_in = const_cast<Bytef*>(input.base);
    strm_->avail_out = static_cast<uInt>(output.size);
    strm_->next_out = output.base;

    auto ret = deflate(strm_, flush);

    NX_ASSERT(ret == Z_OK || ret == Z_STREAM_END || ret == Z_BUF_ERROR,
        "zlib failed");

    wbuf.update_size(output.size - strm_->avail_out);
    rbuf.update_size(-(input.size - strm_->avail_in));
    return ret == Z_STREAM_END;
}

bool unzlib::transform(ringbuf& rbuf, ringbuf& wbuf, bool upstream_eof)
{
    auto* strm_ = reinterpret_cast<z_stream*>(zlib_ctx_);

    auto input = rbuf.continuous_elems();
    auto output = wbuf.continuous_slots();

    strm_->avail_in = static_cast<uInt>(input.size);
    int flush = (upstream_eof && rbuf.empty()) ? Z_FINISH : Z_NO_FLUSH;
    strm_->next_in = const_cast<Bytef*>(input.base);
    strm_->avail_out = static_cast<uInt>(output.size);
    strm_->next_out = output.base;

    auto ret = inflate(strm_, flush);

    NX_ASSERT(ret == Z_OK || ret == Z_STREAM_END || ret == Z_BUF_ERROR,
        "unzlib failed: %d", ret);
    unused(ret);

    wbuf.update_size(output.size - strm_->avail_out);
    rbuf.update_size(-(input.size - strm_->avail_in));
    return ret == Z_STREAM_END;
}

namespace {

void zlib_setup(z_stream* strm, int level)
{
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;

    auto ret = deflateInit(strm, level);
    NX_ASSERT(ret == Z_OK, "zlib compress deflateInit failed");
}

void zlib_cleanup(z_stream* strm) { deflateEnd(strm); }

void unzlib_setup(z_stream* strm)
{
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;

    auto ret = inflateInit(strm);
    NX_ASSERT(ret == Z_OK, "zlib compress inflateInit failed");
}

void unzlib_cleanup(z_stream* strm) { inflateEnd(strm); }

} // namespace

} // namespace nx::io::filter
