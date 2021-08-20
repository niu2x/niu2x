#ifndef NXLUA_PIPE_H
#define NXLUA_PIPE_H

namespace nxlua {

struct filter {
    enum {
        hex,
        unhex,
        zlib,
        unzlib,

    };
};

std::string pipe(const std::string& src, int filter);
std::string pipe_digest(const std::string& src, const char* algorithm);

} // namespace nxlua

#endif