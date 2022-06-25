#ifndef NX_ERRCODE_H
#define NX_ERRCODE_H

namespace nx {
enum errcode {
    E_OK = 0,
    E_CRYPTO_ALGORITHM,
    E_CRYPTO_CONTEXT,
    E_CRYPTO_UPDATE,
    E_CRYPTO_FINAL,
};
}

#endif