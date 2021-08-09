#ifndef NX_MISC_NET_UTILS_H
#define NX_MISC_NET_UTILS_H

#include <niu2x/utils.h>

#if defined(__linux__)
    #include <sys/socket.h>
    #include <arpa/inet.h>
#endif

#if defined(_WIN32) || defined(_WIN64)
    #include <Winsock2.h>
    #include <winsock.h>
    #include <ws2tcpip.h>
#endif

namespace nx::misc {

struct net_utils {
    static std::string desc(const struct sockaddr* sa);
};

} // namespace nx::misc

#endif