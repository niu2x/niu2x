#include <niu2x/async/io_context.h>
#include <niu2x/async/thread_group.h>

namespace nx::async {

io_context_t::io_context_t()
: keep_alive_(false)
, closed_(false)
, active_(0)
, busy_(0)
{
}
io_context_t::~io_context_t() { printf("!io_context_t\n"); }

void io_context_t::post(task_t task)
{
    std::lock_guard<std::mutex> _(tasks_mutex_);
    if (closed_) {
        return;
    }
    tasks_.push_back(std::move(task));
    tasks_cv_.notify_one();
}

int io_context_t::tasks()
{
    std::lock_guard<std::mutex> _(tasks_mutex_);
    return tasks_.size();
}

void io_context_t::run()
{
    std::unique_lock<std::mutex> lk(tasks_mutex_);
    active_++;

    while (true) {
        while (tasks_.size() > 0) {
            auto task = std::move(tasks_.front());
            tasks_.pop_front();

            busy_++;
            lk.unlock();
            task();
            lk.lock();
            busy_--;
        }

        if (keep_alive_ && (!closed_)) {
            tasks_cv_.wait(lk, [this] { return tasks_.size() > 0 || closed_; });
        } else {
            break;
        }
    }

    active_--;
}

int io_context_t::active()
{
    std::lock_guard<std::mutex> _(tasks_mutex_);
    return active_;
}

bool io_context_t::busy()
{
    std::lock_guard<std::mutex> _(tasks_mutex_);
    return busy_ > 0 || tasks_.size() > 0;
}

void io_context_t::stop()
{
    std::lock_guard<std::mutex> _(tasks_mutex_);
    closed_ = true;
    tasks_cv_.notify_all();
}

void io_context_t::set_keep_alive(bool b)
{
    std::lock_guard<std::mutex> _(tasks_mutex_);
    keep_alive_ = b;
}

} // namespace nx::async
