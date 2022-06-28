#include <niu2x/async/http.h>
#include <curl/curl.h>
#include <niu2x/utils.h>
#include <niu2x/async/thread_group.h>

namespace nx::async {

void curl_context_t::run()
{
    auto curl = curl_easy_init();
    NX_FAIL_COND_MSG(!curl, "curl is nullptr");
    io_context_t::run(curl);
    curl_easy_cleanup(curl);
}

static curl_context_t default_context;
static thread_group_t default_thread_group;
static int connection_timeout_ms = 0;
static int timeout_ms = 0;
static int http_version = CURL_HTTP_VERSION_1_1;

static int blackhole(void* data, size_t size, size_t nmemb, void* arg)
{
    unused(data, size, nmemb, arg);
    return size * nmemb;
}

static int write_data(void* data, size_t size, size_t nmemb, void* arg)
{
    size_t len = size * nmemb;
    auto* sink = (std::stringstream*)arg;
    sink->write((const char*)data, len);
    return len;
}

static void do_http_get(CURL* curl, const std::string& url, callback_t callback)
{
    curl_easy_reset(curl);

#define SETOPT(opt, value) curl_easy_setopt(curl, opt, value)

    SETOPT(CURLOPT_URL, url.c_str());
    SETOPT(CURLOPT_HTTP_VERSION, http_version);
    SETOPT(CURLOPT_FOLLOWLOCATION, 1);

    if (connection_timeout_ms)
        SETOPT(CURLOPT_CONNECTTIMEOUT_MS, connection_timeout_ms);
    if (timeout_ms)
        SETOPT(CURLOPT_TIMEOUT_MS, timeout_ms);

    struct curl_slist* chunk = NULL;
    chunk = curl_slist_append(chunk, "Connection: close");
    SETOPT(CURLOPT_HTTPHEADER, chunk);
    NX_FUNC_AT_EXIT(chunk_cleanup, [chunk]() { curl_slist_free_all(chunk); });

    std::stringstream sink;

    SETOPT(CURLOPT_WRITEFUNCTION, write_data);
    SETOPT(CURLOPT_WRITEDATA, &sink);
    SETOPT(CURLOPT_HEADERFUNCTION, blackhole);
    SETOPT(CURLOPT_WRITEHEADER, nullptr);
    SETOPT(CURLOPT_NOSIGNAL, 1L);
    SETOPT(CURLOPT_HTTP_CONTENT_DECODING, 1);
    // SETOPT(CURLOPT_SSL_VERIFYHOST, 0);
    // SETOPT(CURLOPT_SSL_VERIFYPEER, 0);
#undef SETOPT

    CURLcode retCode = curl_easy_perform(curl);
    if (retCode != CURLE_OK) {
        callback(-E_ASYNC_CURL, curl_easy_strerror(retCode));
        return;
    }

    int http_code = 0;
    retCode = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    if (retCode != CURLE_OK) {
        callback(-E_ASYNC_CURL, curl_easy_strerror(retCode));
        return;
    }
    if (http_code != 200 && http_code != 204) {
        callback(-E_ASYNC_HTTP, std::to_string(http_code));
        return;
    }

    callback(E_OK, sink.str());
}

void http_get(std::string url, callback_t callback, curl_context_t* context)
{
    if (!context) {
        context = &default_context;
    }
    context->post(
        [url = std::move(url), callback = std::move(callback)](void* data) {
            CURL* curl = (CURL*)(data);
            do_http_get(curl, url, std::move(callback));
        });
}

void setup() { curl_global_init(CURL_GLOBAL_DEFAULT); }
void cleanup() { curl_global_cleanup(); }

void http_ensure_worker(int n)
{
    for (int i = default_context.active(); i < n; i++) {
        default_thread_group.add_thread([]() { default_context.run(); });
    }
}

void http_wait() { default_thread_group.join_all(); }

} // namespace nx::async