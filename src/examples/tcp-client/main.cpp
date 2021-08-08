#include <niu2x/aio.h>

using namespace nx;

int main()
{
    aio::event_loop e;

    auto tcp = e.create_tcp();
    e.connect(tcp, "127.0.0.1", 9004, [&e](auto status, auto tcp) {
        if (status == ok) {
            e.start_read(tcp,
                [&e](
                    auto status, auto tcp, const uint8_t* buffer, size_t size) {
                    if (status == ok) {
                        std::string s(buffer, buffer + size);
                        std::cout << s;
                    } else {
                        e.stop_read(tcp);
                    }
                });
        } else {
            NX_LOG_F("connect failed");
        }
    });

    e.run(aio::wait_all);

    e.destroy_tcp(tcp);

    return 0;
}