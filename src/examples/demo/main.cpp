#include <iostream>

#define NX_LOG_LEVEL 5

#include <niu2x/memory.h>
#include <niu2x/io.h>
#include <niu2x/aio.h>

using namespace nx;

int main()
{
    auto tcp = aio::create_tcp();
    aio::tcp_connect(tcp, "127.0.0.1", 1994, [](auto status, auto self) {
        if (status != ok) {
            aio::destroy_tcp(self);
            printf("connect fail\n");
            return;
        }

        aio::tcp_write(self, (const uint8_t*)"hello world\n", 12,
            [](auto status, auto self) {
                if (status != ok) {
                    printf("write fail\n");
                    aio::tcp_read_stop(self);
                    aio::destroy_tcp(self);
                } else {
                    aio::tcp_read_stop(self);
                    aio::destroy_tcp(self);
                }
            });
    });
    aio::run();
    return 0;
}