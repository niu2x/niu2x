#include <niu2x/io.h>

#include <zlib.h>

namespace nx::io::filter {

static constexpr size_t CHUNK = 16_k;

static int def(FILE* source, FILE* dest, int level)
{
    int ret, flush;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];

    /* allocate deflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    ret = deflateInit(&strm, level);
    if (ret != Z_OK)
        return ret;

    /* compress until end of file */
    do {
        strm.avail_in = fread(in, 1, CHUNK, source);
        if (ferror(source)) {
            (void)deflateEnd(&strm);
            return Z_ERRNO;
        }
        flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = in;

         // run deflate() on input until output buffer not full, finish
         //   compression if all of source has been read in
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = deflate(&strm, flush); /* no bad return value */
            // assert(ret != Z_STREAM_ERROR); /* state not clobbered */

            have = CHUNK - strm.avail_out;
            if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
                (void)deflateEnd(&strm);
                return Z_ERRNO;
            }
        } while (strm.avail_out == 0);
        // assert(strm.avail_in == 0); /* all input will be used */

        /* done when last data in file processed */
    } while (flush != Z_FINISH);
    // assert(ret == Z_STREAM_END); /* stream will be complete */

    /* clean up and return */
    (void)deflateEnd(&strm);
    return Z_OK;
}

// void encrypt_utils::zlib_compress(
//     const uint8_t* input, size_t len, std::vector<uint8_t>& output)
// {
//     unused(input, len, output);
// }

// void encrypt_utils::zlib_uncompress(
//     const uint8_t* input, size_t len, std::vector<uint8_t>& output)
// {
//     unused(input, len, output);
// }

struct zlib_compress_t::context  {

};


status zlib_compress_t::cvt(const uint8_t* input, size_t isize,
        size_t* consumed_isize, uint8_t* output, size_t max_osize,
        size_t* osize)  {
	if(!ctx_){
		ctx_ = new context;
	}
}


zlib_compress_t::zlib_compress_t(const zlib_compress_t &other)
:ctx_(nullptr){
	unused(other);
}
// zlib_compress_t& zlib_compress_t::operator=(const zlib_compress_t &other) {
// 	if(this != &other){
// 		ctx_ = nullptr;
// 	}
// 	return *this;
// }


zlib_compress_t::zlib_compress_t() 
    :ctx_(nullptr){}

zlib_compress_t::~zlib_compress_t() {
	NX_DELETE(ctx_);
}

}