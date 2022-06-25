#ifndef NX_CRYPTO_HEX_H
#define NX_CRYPTO_HEX_H

#include <string>
#include <niu2x/utils.h>

namespace nx::crypto {

NXAPI int hex_size(const void* input, int size);
NXAPI int hex(void* out, const void* input, int size);
NXAPI std::string hex(const void* input, int size);
NXAPI std::string hex(const std::string&);

NXAPI int unhex_size(const void* input, int size);
NXAPI int unhex(void* out, const void* input, int size);
NXAPI std::string unhex(const void* input, int size);
NXAPI std::string unhex(const std::string&);

} // namespace nx::crypto

#endif