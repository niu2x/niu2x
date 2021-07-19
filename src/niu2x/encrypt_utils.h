#ifndef NX_ENCRYPT_UTILS_H
#define NX_ENCRYPT_UTILS_H

#include <niu2x/utils.h>

namespace nx {

struct encrypt_utils {
	void hex_encode(const uint8_t *input, size_t len, uint8_t *output);
	void hex_decode(const uint8_t *input, uint8_t *output);

	void md5(const uint8_t *input, size_t len, uint8_t *output);
	void md5_file(const char *pathname, uint8_t *output);

	void zlib_compress(const uint8_t *input, size_t len, std::vector<uint8_t> &output);
	void zlib_uncompress(const uint8_t *input, size_t len, std::vector<uint8_t> &output);
};

}

#endif