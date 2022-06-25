#include <string.h>
#include <stdlib.h>
#include <cstdint>
#include <stdio.h>

#include <niu2x/utils.h>
#include <niu2x/crypto.h>

int main(int argc, char* argv[])
{
    const char* message = "hello world\n";

    int max_size = nx::crypto::md5_size(message, strlen(message));
    uint8_t* md5_buf = (uint8_t*)malloc(max_size);

    int size = nx::crypto::md5(md5_buf, message, strlen(message));

    for (int i = 0; i < size; i++) {
        printf("%02x", md5_buf[i]);
    }
    printf("\n");

    free(md5_buf);
}