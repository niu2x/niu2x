#include <iostream>

#define NX_LOG_LEVEL 5

#include <niu2x/memory.h>
#include <niu2x/io.h>
#include <niu2x/aio.h>
#include <niu2x/lexical_cast.h>

using namespace nx;

int main()
{
    auto tcp = aio::create_tcp();
    // aio::tcp_connect(tcp, "127.0.0.1", 1994, [](auto status, auto self) {
    //     if (status != ok) {
    //         aio::destroy_tcp(self);
    //         printf("connect fail\n");
    //         return;
    //     }

    //     aio::tcp_write(self, (const uint8_t*)"hello world\n", 12,
    //         [](auto status, auto self) {
    //             if (status != ok) {
    //                 printf("write fail\n");
    //                 aio::tcp_read_stop(self);
    //                 aio::destroy_tcp(self);
    //             } else {
    //                 aio::tcp_read_stop(self);
    //                 aio::destroy_tcp(self);
    //             }
    //         });
    // });

    // aio::tcp_listen(tcp, "0.0.0.0", 8888, [](auto status, auto id) {
    //     if (status != ok) {
    //         NX_LOG_E("callback is not ok");
    //         aio::destroy_tcp(id);
    //     } else {

    //         aio::create_idle([id](auto idle) {
    //             if (!aio::tcp_alive(id)) {
    //                 aio::destroy_idle(idle);
    //                 return;
    //             }

    //             aio::tcp_write(
    //                 id, "hello world", 11, [](auto status, auto self) {
    //                     if (status != ok) {
    //                         aio::destroy_tcp(self);
    //                     }
    //                 });
    //         });
    //     }
    // });

    // aio::run();

    std::cout << lexical_cast<int>("2313f", 4) << std::endl;

    return 0;
}