#include <niu2x/io.h>

namespace nx::io {

io::sink_adapter<uint8_t, std::ostream> cout(std::cout);
io::sink_adapter<uint8_t, std::ostream> cerr(std::cerr);

static size_t file_size(FILE* fp) noexcept
{
    auto old_pos = ftell(fp);
    fseek(fp, 0, SEEK_END);
    auto size = ftell(fp);
    fseek(fp, old_pos, SEEK_SET);
    return size;
}

void read_file(const char* pathname, std::vector<uint8_t>& output)
{
    FILE* fp = nullptr;

#if defined(_WIN32) || defined(_WIN64)
    NX_ASSERT(!fopen_s(&fp, pathname, "rb"), "open file failed");
#else
    fp = fopen(pathname, "rb");
    NX_ASSERT(fp, "open file failed");
#endif

    auto fsize = file_size(fp);
    output.resize(fsize);

    NX_ASSERT(fread(output.data(), 1, fsize, fp) == fsize, "read file failed.");
}

} // namespace nx::io

namespace nx::io::filter {

simple_filter<uint8_t, uint8_t> lower { [](const uint8_t& c) {
    return (uint8_t)tolower(c);
} };

simple_filter<uint8_t, uint8_t> upper { [](const uint8_t& c) {
    return (uint8_t)toupper(c);
} };

simple_filter<uint8_t, uint8_t> inc { [](const uint8_t& c) { return (uint8_t)(c + 1); } };

static uint8_t hex(uint8_t digit)
{
    if (digit < 10)
        return '0' + digit;
    return 'a' + digit - 10;
}

status hex_encode_t::cvt(const uint8_t* input, size_t isize,
    size_t* consumed_isize, uint8_t* output, size_t max_osize, size_t* osize)
{

    if (!isize)
        return status::eof;

    size_t readn = min(isize, max_osize >> 1);

    if (readn) {

        if (consumed_isize)
            *consumed_isize = readn;
        if (osize)
            *osize = readn << 1;

        for (const uint8_t* end = input + readn; input < end; input++) {
            uint8_t c = *input;
            *output++ = hex(c / 10);
            *output++ = hex(c % 10);
        }

        return status::ok;

    } else {
        return status::again;
    }
}

hex_encode_t hex_encode;

} // namespace nx::io::filter