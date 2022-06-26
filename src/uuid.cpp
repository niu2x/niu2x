#include <niu2x/uuid.h>

#include <sstream>
#include <boost/uuid/uuid_generators.hpp>

namespace nx {

uuid_t uuid_gen() { return boost::uuids::random_generator()(); }

NXAPI std::string uuid_str_gen()
{
    auto uuid = uuid_gen();
    return uuid_str(&uuid);
}

static char hex_char(int n)
{
    static char digits[] = "0123456789abcdef";
    return digits[n];
}

NXAPI std::string uuid_str(const uuid_t* self)
{
    int split_pos[] = { 4, 2, 2, 2, 0 };
    int head = 0;

    std::stringstream ss;
    for (auto it = self->begin(); it != self->end(); it++) {
        uint8_t c = *it;
        ss << hex_char(c >> 4);
        ss << hex_char(c & 0xF);

        if (--split_pos[head] == 0) {
            ss << '-';
            head++;
        }
    }
    return ss.str();
}

} // namespace nx