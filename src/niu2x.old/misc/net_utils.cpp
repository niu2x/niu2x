#include "net_utils.h"

#if defined(_WIN32) || defined(_WIN64)
    #include <Winsock2.h>
    #include <winsock.h>
    #include <ws2tcpip.h>
#endif

#if !defined(_WIN32) || !defined(_WIN64)
    #define strncpy_s strncpy
#endif

namespace nx::misc {

std::string net_utils::desc(const struct sockaddr* sa)
{
    std::stringstream ss;
    char address[INET_ADDRSTRLEN];
    uint16_t port = 0;
    switch (sa->sa_family) {
        case AF_INET:
            inet_ntop(AF_INET, &(((struct sockaddr_in*)sa)->sin_addr), address,
                INET_ADDRSTRLEN);
            port = ((struct sockaddr_in*)sa)->sin_port;
            break;

        case AF_INET6:
            inet_ntop(AF_INET6, &(((struct sockaddr_in6*)sa)->sin6_addr),
                address, INET_ADDRSTRLEN);
            port = ((struct sockaddr_in6*)sa)->sin6_port;
            break;

        default:
            strncpy_s(address, "Unknown AF", INET_ADDRSTRLEN);
            return NULL;
    }
    ss << address;
    ss << " ";
    ss << ntohs(port);

    return ss.str();
}

} // namespace nx::misc