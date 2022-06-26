#include <niu2x/async/thread_group.h>

#include <algorithm>
#include <sstream>

namespace nx::async {

thread_group_t::thread_group_t() { }
thread_group_t::~thread_group_t() { join_all(); }

void thread_group_t::_add_thread(std::thread&& th)
{
    std::stringstream os;
    os << th.get_id();
    {
        lock_t _(cls_mutex_);
        threads_.push_back(std::move(th));
    }
}

void thread_group_t::join_all()
{
    std::decay<decltype(threads_)>::type threads;
    {
        lock_t _(cls_mutex_);
        threads = std::move(threads_);
    }

    for (auto& th : threads) {
        if (th.joinable()) {
            th.join();
        }
    }
}

void thread_group_t::_remove_thread(const std::thread::id& tid)
{
    lock_t _(cls_mutex_);

    std::stringstream os;
    os << tid;
    auto it = std::find_if(threads_.begin(), threads_.end(),
        [&tid](std::thread& v) { return v.get_id() == tid; });

    if (it != threads_.end()) {
        it->detach();
        threads_.erase(it);
    }
}
} // namespace nx::async