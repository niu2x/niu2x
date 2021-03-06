#ifndef NX_ASYNC_THREAD_GROUP_H
#define NX_ASYNC_THREAD_GROUP_H

#include <thread>
#include <functional>
#include <mutex>
#include <list>
#include <boost/noncopyable.hpp>
#include <niu2x/api.h>

namespace nx::async {

class NXAPI thread_group_t : private boost::noncopyable {
public:
    thread_group_t();
    ~thread_group_t();

    template <class Func, class... Args>
    void add_thread(Func&& func, Args&&... args)
    {
        _add_thread(std::thread([=]() {
            func(std::forward<Args>(args)...);
            _remove_thread(std::this_thread::get_id());
        }));
    }

    void join_all();

private:
    std::mutex cls_mutex_;
    using lock_t = std::lock_guard<std::mutex>;

    void _add_thread(std::thread&& th);
    void _remove_thread(const std::thread::id& tid);
    std::list<std::thread> threads_;
};

} // namespace nx::async

#endif