#include <niu2x/uuid.h>
#include <niu2x/utils.h>

#include <sstream>
#include <random>
#include <sstream>

#include "uuid.h"

namespace nx {

uuid_t uuid_gen()
{
    std::random_device rd;
    auto seed_data = std::array<int, 6> {};
    std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    std::ranlux48_base generator(seq);

    uuids::basic_uuid_random_generator<std::ranlux48_base> gen(&generator);
    auto id = gen();
    auto bytes = id.as_bytes();

    uuid_t uuid;
    for (int i = 0; i < bytes.size(); i++)
        uuid.bits[i] = (uint8_t)bytes[i];

    return uuid;
}

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
    for (auto it = self->bits; it != self->bits + uuid_t::size; it++) {
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