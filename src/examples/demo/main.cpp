#include <iostream>

#define NX_LOG_LEVEL 5

#include <niu2x/memory.h>
#include <niu2x/io.h>
#include <niu2x/aio.h>

using namespace nx;

int main()
{

    // nx::aio::create_idle([](auto rid) {
    //     printf("hello world!\n");
    //     nx::aio::destroy_idle(rid);
    // });
    //

    auto tcp = nx::aio::create_tcp();
    nx::aio::tcp_connect(tcp, "127.0.0.1", 1994, [](auto status, auto self) {
        if (status != ok) {
            nx::aio::destroy_tcp(self);
            return;
        }

        if (ok
            != nx::aio::tcp_read_start(
                self, [](auto status, auto self, const uint8_t* buf, size_t s) {
                    if (status == ok) {
                        printf("%s", std::string(buf, buf + s).c_str());
                        if (std::string(buf, buf + s) == "quit\n") {
                            nx::aio::tcp_read_stop(self);
                            nx::aio::destroy_tcp(self);
                        }
                    } else {
                        nx::aio::tcp_read_stop(self);
                        nx::aio::destroy_tcp(self);
                    }
                })) {
            nx::aio::destroy_tcp(self);
            return;
        }
    });
    nx::aio::run();
    return 0;
}