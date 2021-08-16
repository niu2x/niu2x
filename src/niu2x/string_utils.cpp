#include <niu2x/string_utils.h>

#include <string.h>

#include <sstream>

#if !defined(_WIN32) || !defined(_WIN64)
    #define strncpy_s strncpy
#endif

namespace nx {

std::vector<std::string> string_utils::split(
    const std::string_view& sz, uint8_t delimiter)
{

    std::vector<std::string> results;
    size_t pos = 0, prev_pos = 0;
    std::string token;
    while ((pos = sz.find(delimiter, prev_pos)) != std::string_view::npos) {
        results.emplace_back(sz.substr(prev_pos, pos - prev_pos));
        prev_pos = pos + 1;
    }

    if (prev_pos < sz.length()) {
        results.emplace_back(sz.substr(prev_pos, sz.length() - prev_pos));
    } else {
        results.emplace_back("");
    }

    return results;
}

std::string string_utils::string(const struct sockaddr* sa)
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

} // namespace nx
