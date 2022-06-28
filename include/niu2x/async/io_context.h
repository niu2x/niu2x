#ifndef NX_ASYNC_IO_CONTEXT_H
#define NX_ASYNC_IO_CONTEXT_H

#include <functional>
#include <mutex>
#include <list>
#include <condition_variable>
#include <boost/noncopyable.hpp>

#include <niu2x/api.h>

namespace nx::async {

class NXAPI io_context_t : private boost::noncopyable {
public:
    using task_t = std::function<void(void*)>;

    io_context_t();
    ~io_context_t();

    io_context_t(io_context_t&&) = default;
    io_context_t& operator=(io_context_t&&) = default;

    void run(void* data);
    void stop();

    void post(task_t task);

    void set_keep_alive(bool b);

    int tasks();
    int active();
    bool busy();

private:
    std::list<task_t> tasks_;
    std::condition_variable tasks_cv_;
    std::mutex tasks_mutex_;
    bool keep_alive_;
    bool closed_;
    int active_;
    int busy_;
};

} // namespace nx::async
#endif