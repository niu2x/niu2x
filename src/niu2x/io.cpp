#include <niu2x/io.h>


namespace nx::io {

size_t file_size(FILE *fp) noexcept {
	auto old_pos = ftell(fp);
	fseek(fp, 0, SEEK_END);
	auto size = ftell(fp);
	fseek(fp, old_pos, SEEK_SET);
	return size;
}

void read_file(const char *pathname, std::vector<uint8_t> &output) {
	FILE *fp = nullptr;
	
	NX_ASSERT(!fopen_s( &fp, pathname, "rb"), "open file failed");

	auto fsize = file_size(fp);
	output.resize(fsize);

	NX_ASSERT(fread(output.data(), 1, fsize, fp) == fsize, "read file failed.");
}

}

