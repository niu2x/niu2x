#include <niu2x/io.h>

namespace nx::io {

source::source() { }
source::~source() { }

sink::sink() { }
sink::~sink() { }

static sink_null my_sink_null;
sink_proxy null(&my_sink_null);

} // namespace nx::io