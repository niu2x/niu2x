#ifndef NX_STRING_UTILS_H
#define NX_STRING_UTILS_H

#if defined(__linux__)
    #include <sys/socket.h>
    #include <arpa/inet.h>
#endif

#if defined(_WIN32) || defined(_WIN64)
    #include <Winsock2.h>
    #include <winsock.h>
    #include <ws2tcpip.h>
#endif

#include <string>
#include <vector>
#include <string_view>

#include <niu2x/api.h>

namespace nx {

struct API string_utils {
    static std::vector<std::string> split(
        const std::string_view& sz, uint8_t delimiter);

    static std::string string(const struct sockaddr* sa);
};

} // namespace nx

#endif