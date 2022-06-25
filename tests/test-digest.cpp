#include <string.h>
#include <stdlib.h>
#include <cstdint>
#include <string>
#include <stdio.h>

#include <niu2x/utils.h>
#include <niu2x/crypto.h>

int main(int argc, char* argv[])
{
    const char* message = "hello world\nhello world\nhello world\n";

    int max_size = nx::crypto::md5_size(message, strlen(message));
    uint8_t* md5_buf = (uint8_t*)malloc(max_size);

    int size = nx::crypto::md5(md5_buf, message, strlen(message));

    for (int i = 0; i < size; i++) {
        printf("%02x", md5_buf[i]);
    }
    printf("\n");
    free(md5_buf);

    std::string out
        = nx::crypto::aes_256_cbc(nx::crypto::encrypt, message, strlen(message),
            "00000000000000000000000000000000", "1111111111111111");

    for (int i = 0; i < out.size(); i++) {
        printf("%02x", (uint8_t)out[i]);
    }
}