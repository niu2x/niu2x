#ifndef NXLUA_PIPE_H
#define NXLUA_PIPE_H

namespace nxlua {

struct pipe_filter {
    enum {
        hex,
        unhex,
        zlib,
        unzlib,

    };
};

struct pipe_cipher_mode {
    enum { decrypt = 0, encrypt = 1 };
};

std::string pipe(const std::string& src, int filter);
std::string pipe_digest(const std::string& src, const char* algorithm);
std::string pipe_cipher(const std::string& src, int mode, const char* algorithm,
    const std::string& key, const std::string& iv);
} // namespace nxlua

#endif