// #include "aio.h"

// #include <uv.h>
// #include <niu2x/misc/freelist.h>
// #include <niu2x/misc/net_utils.h>
// #include <niu2x/log.h>

// // #include <niu2x/global.h>

// namespace nx::aio {

// namespace {

// struct idle {
//     uv_idle_t uv_obj;
//     union {
//         event_loop::idle_handle callback;
//     };
// };

// struct stream {
//     union {
//         uv_tcp_t uv_tcp;
//     };
//     enum {
//         tcp,
//         udp,
//     } type;

//     union {
//         event_loop::read_handle read_cb;
//     };
// };

// struct connect_t {
//     uv_connect_t uv_obj;
//     union {
//         event_loop::connect_handle callback;
//     };
//     struct sockaddr_in dest;
// };

// class uv_event_loop : public event_loop, private noncopyable {
// public:
//     uv_event_loop()
//     {
//         NX_LOG_D("uv_event_loop::uv_event_loop");
//         uv_loop_init(&loop_);
//         loop_.data = this;
//     }
//     virtual ~uv_event_loop()
//     {
//         uv_loop_close(&loop_);
//         NX_LOG_D("uv_event_loop::~uv_event_loop");
//     }

//     virtual void run(int p_type) override
//     {
//         switch (p_type) {
//             case wait_all: {
//                 uv_run(&loop_, UV_RUN_DEFAULT);
//                 break;
//             }
//             case wait_once: {
//                 uv_run(&loop_, UV_RUN_ONCE);
//                 break;
//             }
//             case no_wait: {
//                 uv_run(&loop_, UV_RUN_NOWAIT);
//                 break;
//             }
//         }
//     }

//     virtual rid create_idle(idle_handle handle) override
//     {
//         rid idle_id = idles_.alloc();
//         if (idle_id != rid::nil) {
//             auto* idle = idles_.get(idle_id);
//             uv_idle_init(&loop_, &(idle->uv_obj));
//             idle->uv_obj.data = reinterpret_cast<void*>(idle_id.id);
//             uv_idle_start(&(idle->uv_obj), idle_callback);
//             new (&(idle->callback)) event_loop::idle_handle(handle);
//         }

//         return idle_id;
//     }
//     virtual void destroy_idle(rid idle_id) override
//     {
//         if (idle_id == rid::nil)
//             return;
//         auto* idle = idles_.get(idle_id);
//         NX_ASSERT(idle, "invalid idle ptr");
//         destructor<event_loop::idle_handle>::destory(&(idle->callback));
//         uv_idle_stop(&(idle->uv_obj));
//         uv_close(reinterpret_cast<uv_handle_t*>(&(idle->uv_obj)), nullptr);
//         idles_.free(idle_id);
//     }

//     virtual rid create_tcp() override
//     {
//         rid tcp_id = streams_.alloc();
//         if (tcp_id != rid::nil) {
//             auto* tcp = streams_.get(tcp_id);
//             tcp->type = stream::tcp;
//             new (&(tcp->read_cb)) event_loop::read_handle();
//             uv_tcp_init(&loop_, &(tcp->uv_tcp));
//             tcp->uv_tcp.data = reinterpret_cast<void*>(tcp_id.id);
//         }

//         NX_LOG_D("create_tcp %u", tcp_id.id);
//         return tcp_id;
//     }

//     virtual void destory_tcp(rid tcp_id) override
//     {
//         NX_LOG_D("destory_tcp %u", tcp_id.id);
//         if (tcp_id == rid::nil)
//             return;
//         auto* tcp = streams_.get(tcp_id);
//         NX_ASSERT(tcp, "invalid tcp ptr");
//         uv_close(reinterpret_cast<uv_handle_t*>(&(tcp->uv_tcp)), nullptr);
//         destructor<event_loop::read_handle>::destory(&(tcp->read_cb));
//         streams_.free(tcp_id);
//     }

//     virtual void connect(rid tcp_id, const char* address, uint16_t port,
//         connect_handle callback) override
//     {
//         if (create_connect(tcp_id, address, port, callback) == rid::nil) {
//             // todo delay callback
//             NX_LOG_E("tcp failed to connect: %s %d", address, port);
//             callback(fail, tcp_id);
//         }
//     }

//     virtual void start_read(rid stream_id, read_handle callback) override
//     {
//         if (stream_id != rid::nil) {
//             auto* my_stream = streams_.get(stream_id);
//             if (my_stream->read_cb != nullptr) {
//                 NX_LOG_W("steam is already start read");
//                 return;
//             }

//             my_stream->read_cb = callback;
//             switch (my_stream->type) {
//                 case stream::tcp: {
//                     auto s =
//                     uv_read_start((uv_stream_t*)&(my_stream->uv_tcp),
//                         my_alloc_cb, read_callback);
//                     if (s) {
//                         NX_LOG_E("tcp failed to start_read: %d, reason: %s",
//                         s,
//                             uv_strerror(s));
//                     }

//                     break;
//                 }
//                 default: {
//                 }
//             }
//         }
//     }

//     virtual void stop_read(rid stream_id) override
//     {
//         if (stream_id != rid::nil) {
//             auto* my_stream = streams_.get(stream_id);
//             switch (my_stream->type) {
//                 case stream::tcp: {
//                     uv_read_stop((uv_stream_t*)&(my_stream->uv_tcp));
//                     break;
//                 }
//                 default: {
//                 }
//             }
//         }
//     }

// private:
//     uv_loop_t loop_;

//     freelist<idle, 4> idles_;
//     static void idle_callback(uv_idle_t* handle)
//     {
//         rid idle_id { rid::id_t(uint64_t(handle->data)) };
//         NX_ASSERT(idle_id != rid::nil, "can't find this handle");
//         uv_loop_t* uv_loop = handle->loop;
//         auto* my_loop = reinterpret_cast<uv_event_loop*>(uv_loop->data);
//         auto* idle = my_loop->idles_.get(idle_id);
//         idle->callback(idle_id);
//     }

//     freelist<stream, 1024> streams_;
//     misc::freelist<connect_t, 1024> connects_;

//     rid create_connect(
//         rid tcp_id, const char* address, uint16_t port, connect_handle
//         callback)
//     {
//         rid con_id = connects_.alloc();
//         if (con_id != rid::nil) {
//             auto* con = connects_.get(con_id);
//             con->uv_obj.data = reinterpret_cast<void*>(con_id.id);
//             new (&(con->callback)) event_loop::connect_handle(callback);
//             struct sockaddr_in* p_dest = &(con->dest);
//             uv_ip4_addr(address, port, p_dest);
//             uv_tcp_connect(&(con->uv_obj), &(streams_.get(tcp_id)->uv_tcp),
//                 (const struct sockaddr*)p_dest, connect_callback);
//         }
//         return con_id;
//     }

//     void destroy_connect(rid con_id)
//     {
//         if (con_id == rid::nil)
//             return;
//         auto* con = connects_.get(con_id);
//         NX_ASSERT(con, "invalid idle ptr");
//         destructor<event_loop::connect_handle>::destory(&(con->callback));
//         connects_.free(con_id);
//     }

//     static void connect_callback(uv_connect_t* uv_con, int status)
//     {

//         uv_loop_t* uv_loop = uv_con->handle->loop;
//         auto* my_loop = reinterpret_cast<uv_event_loop*>(uv_loop->data);

//         rid con_id { rid::id_t(uint64_t(uv_con->data)) };

//         uv_tcp_t* uv_tcp = reinterpret_cast<uv_tcp_t*>(uv_con->handle);
//         rid tcp_id { rid::id_t(uint64_t(uv_tcp->data)) };

//         auto* con = my_loop->connects_.get(con_id);

//         if (status < 0) {
//             NX_LOG_E("tcp failed to connect: %s reason: %s",
//                 misc::net_utils::desc((struct
//                 sockaddr*)&(con->dest)).c_str(), uv_strerror(status));
//             con->callback(aio::fail, tcp_id);
//         } else {
//             con->callback(aio::ok, tcp_id);
//         }

//         my_loop->destroy_connect(con_id);
//     }

//     static void my_alloc_cb(
//         uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
//     {
//         unused(handle);
//         buf->base = (char*)global::allocator.allocate(suggested_size);
//         buf->len = suggested_size;
//     }

//     static void free_buf(const uv_buf_t* buf)
//     {
//         if (buf->base && buf->len) {
//             global::allocator.free(buf->base);
//         }
//     }

//     static void read_callback(
//         uv_stream_t* uv_stream, ssize_t nread, const uv_buf_t* buf)
//     {

//         uv_loop_t* uv_loop = uv_stream->loop;
//         auto* my_loop = reinterpret_cast<uv_event_loop*>(uv_loop->data);
//         rid stream_id { rid::id_t(uint64_t(uv_stream->data)) };
//         auto* stream = my_loop->streams_.get(stream_id);

//         unused(stream, stream_id);

//         if (nread < 0) {
//             stream->read_cb(fail, stream_id, nullptr, 0);
//         }

//         else if (nread > 0) {
//             if (stream->read_cb)
//                 stream->read_cb(
//                     ok, stream_id, (const uint8_t*)buf->base, nread);
//         }

//         free_buf(buf);
//     }
// };

// NX_INIT_FUNC(init_uv_allocator) { }

// } // namespace

// } // namespace nx::aio

// namespace nx::aio {

// event_loop::event_loop() { }
// event_loop::~event_loop() { }

// std::unique_ptr<event_loop> event_loop::create()
// {
//     NX_LOG_D("uv_event_loop size: %lu", sizeof(uv_event_loop));
//     return std::make_unique<uv_event_loop>();
// }

// } // namespace nx::aio
