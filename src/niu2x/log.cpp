#include <niu2x/log.h>

#include <stdarg.h>
#include <stdio.h>

namespace nx {

namespace {

    // clang-format off
const char *level_colors[] = { 
    "\x1b[94m", 
    "\x1b[36m", 
    "\x1b[33m", 
    "\x1b[31m", 
    "\x1b[35m"
};

const char *level_names[] = {
    "trace",
    "debug",
    "warning",
    "error",
    "fatal",
};
    // clang-format on

} // namespace

void log::write(
    level p_level, const char* filename, int line, const char* msg, ...)
{

    int lvl = int(p_level);
    auto* fmt = "\x1b[0m\x1b[90m%s[%s] %s %d ";
    fprintf(stderr, fmt, level_colors[lvl], level_names[lvl], filename, line);

    va_list args;
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    fprintf(stderr, "\x1b[0m\n");
    va_end(args);
}

} // namespace nx
