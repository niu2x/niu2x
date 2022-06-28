#ifndef NX_ERRCODE_H
#define NX_ERRCODE_H

namespace nx {
enum errcode {
    E_OK = 0,
    E_BUSY,
    E_EOF,
    E_OPERATION,
    E_CRYPTO_ALGORITHM,
    E_CRYPTO_CONTEXT,
    E_CRYPTO_UPDATE,
    E_CRYPTO_FINAL,
    E_FFMPEG_OPEN,
    E_FFMPEG_NOT_OPEN,
    E_ASYNC_CURL,
    E_ASYNC_HTTP,
};
}

#endif