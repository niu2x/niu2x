#include <niu2x/io.h>

namespace nx::io {

static constexpr size_t tmpsize = 8;

void pipe(source_proxy& src, sink_proxy& dst)
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

} // namespace nx::io