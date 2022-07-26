#ifndef NX_ASYNC_HTTP_H
#define NX_ASYNC_HTTP_H

#include <niu2x/build.h>
#if defined(CURL_FOUND)

    #include <functional>
    #include <string>
    #include <sstream>

    #include <niu2x/api.h>
    #include <niu2x/async/io_context.h>

namespace nx::async {

class NXAPI curl_context_t : public io_context_t {
public:
    using task_t = std::function<void(void*)>;

    curl_context_t() = default;
    ~curl_context_t() = default;
    curl_context_t(curl_context_t&&) = default;
    curl_context_t& operator=(curl_context_t&&) = default;

    void run();

private:
};

using callback_t = std::function<void(int err, std::string content)>;
NXAPI void http_get(
    std::string url, callback_t callback, curl_context_t* context = nullptr);

NXAPI void setup();
NXAPI void cleanup();

NXAPI void http_ensure_worker(int n);
NXAPI void http_wait();

} // namespace nx::async

#endif
#endif