#include <niu2x/io.h>

namespace nx::io {

io::sink_adapter<uint8_t, std::ostream> cout(std::cout);

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
