#ifndef NX_CRYPTO_BASE64_H
#define NX_CRYPTO_BASE64_H

#include <string>
#include <niu2x/utils.h>

namespace nx::crypto {

NXAPI int base64_size(const void* input, int size);
NXAPI int base64(void* out, const void* input, int size);
NXAPI std::string base64(const void* input, int size);
NXAPI std::string base64(const std::string&);

NXAPI int unbase64_size(const void* input, int size);
NXAPI int unbase64(void* out, const void* input, int size);
NXAPI std::string unbase64(const void* input, int size);
NXAPI std::string unbase64(const std::string&);

} // namespace nx::crypto

#endif