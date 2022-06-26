#include <niu2x/algorithm/binary_search.h>
#include <niu2x/utils.h>

namespace nx::algo {

int binary_search(const void* base, const int nr, const int stride,
    const void* target, const compare_t compare)
{

    int start = 0;
    int end = nr;

    while (start < end) {
        int mid = (start + end) >> 1;
        int r = compare(target, NX_ADDRESS_OFFSET(void*, base, stride* mid));
        if (r == 0) {
            return mid;
        } else if (r < 0) {
            end = mid;
        } else {
            start = mid + 1;
        }
    }
    return -start - 1;
}

} // namespace nx::algo