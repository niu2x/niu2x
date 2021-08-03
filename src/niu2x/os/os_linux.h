#ifndef NX_OS_LINUX_H
#define NX_OS_LINUX_H

#include <niu2x/utils.h>

namespace nx::os {

inline API pid_t pid() { return getpid(); }

inline API pid_t ppid() { return getppid(); }

} // namespace nx::os

#endif