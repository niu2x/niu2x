#include <niu2x/io.h>

namespace nx::io {

source::source() { }
source::~source() { }

sink::sink() { }
sink::~sink() { }

static null_sink my_null_sink;
sink_proxy null(&my_null_sink);

} // namespace nx::io