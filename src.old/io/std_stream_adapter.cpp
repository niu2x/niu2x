#include <niu2x/io.h>

namespace nx::io {

std_istream_adapter::std_istream_adapter(std::istream& delegate)
: delegate_(delegate)
{
}

std_istream_adapter::~std_istream_adapter() { }

status std_istream_adapter::get(uint8_t* output, size_t max, size_t& osize)
{
    delegate_.read(reinterpret_cast<char*>(output), max);
    size_t readn;
    if (delegate_)
        readn = max;
    else
        readn = delegate_.gcount();
    osize = readn;
    return readn ? status::ok : status::eof;
}

std_ostream_adapter::std_ostream_adapter(std::ostream& delegate)
: delegate_(delegate)
{
}

std_ostream_adapter::~std_ostream_adapter() { }

status std_ostream_adapter::put(
    const uint8_t* output, size_t max, size_t& osize)
{
    delegate_.write(reinterpret_cast<const char*>(output), max);
    NX_ASSERT(delegate_.good(), "adapter<uint8_t, std::ostream> write failed");
    osize = max;
    return status::ok;
}

static std_ostream_adapter sink_cout(std::cout);
static std_ostream_adapter sink_cerr(std::cerr);
static std_istream_adapter source_cin(std::cin);

sink_proxy cout(&sink_cout);
sink_proxy cerr(&sink_cerr);
source_proxy cin(&source_cin);

} // namespace nx::io