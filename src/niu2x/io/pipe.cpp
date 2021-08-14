#include <niu2x/io.h>

namespace nx::io {

static constexpr size_t tmpsize = 4_k;

template <class output>
static void pipe(source_proxy src, output dst)
{
    uint8_t tmp[tmpsize];
    size_t isize;
    int status = src.get(tmp, tmpsize, isize);
    while (status != eof) {
        if (status == ok) {
            size_t osize;
            uint8_t* write_ptr = tmp;
            do {
                status = dst.put(write_ptr, isize, osize);
                if (status == ok) {
                    write_ptr += osize;
                    isize -= osize;
                }
            } while (status != eof && isize > 0);
        }
        status = src.get(tmp, tmpsize, isize);
    }
}

void pipe(source_proxy src, sink_proxy dst) { pipe<sink_proxy>(src, dst); }

void pipe(source_proxy src, filter_proxy dst) { pipe<filter_proxy>(src, dst); }

void pipe(source_proxy src, filter_proxy filter, sink_proxy dst)
{
    filter.reset();
    filter.set_next(dst);

    pipe(src, filter);
    size_t osize;
    while (filter.put(nullptr, 0, osize) == again)
        ;
}

void pipe(source_proxy src, filter_proxy f0, filter_proxy f1, sink_proxy dst)
{
    f0.reset();
    f1.reset();
    f0.set_next(f1.delegate());
    f1.set_next(dst);

    pipe(src, f0);
    size_t osize;

    while (f0.put(nullptr, 0, osize) == again)
        ;
    while (f1.put(nullptr, 0, osize) == again)
        ;
}

} // namespace nx::io