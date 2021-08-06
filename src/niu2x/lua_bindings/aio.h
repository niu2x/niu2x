#ifndef NX_LUA_LB_AIO_H
#define NX_LUA_LB_AIO_H

#include <niu2x/aio.h>

namespace nx::lua_bindings::aio {

using status = nx::aio::status;

// operator int(const status &s){
//     return 0;
// }

class API event_loop : public nx::aio::event_loop {
public:
    static event_loop create()
    {
        return event_loop(nx::aio::event_loop ::create());
    }

    event_loop(std::unique_ptr<nx::aio::event_loop>&& delegate);
    virtual ~event_loop();

    virtual void run(type p_type) override { delegate_->run(p_type); }

    void run(int p_type) { run((type)p_type); }
    void run() { run(0); }

    virtual rid create_idle(idle_handle handle) override
    {
        return delegate_->create_idle(handle);
    }
    virtual void destroy_idle(rid p) override { delegate_->destroy_idle(p); }

    virtual rid create_tcp() override { return delegate_->create_tcp(); }
    virtual void destory_tcp(rid tcp_id) override
    {
        delegate_->destory_tcp(tcp_id);
    }

    virtual void connect(rid tcp_id, const char* address, uint16_t port,
        connect_handle cb) override
    {
        delegate_->connect(tcp_id, address, port, cb);
    }

    virtual void start_read(rid stream_id, read_handle callback) override
    {
        delegate_->start_read(stream_id, callback);
    }
    virtual void stop_read(rid stream_id) override
    {
        delegate_->stop_read(stream_id);
    }

private:
    std::shared_ptr<nx::aio::event_loop> delegate_;
};

} // namespace nx::lua_bindings::aio

#endif