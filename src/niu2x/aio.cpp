#include <niu2x/aio.h>

#include <uv.h>

#define LOOP uv_default_loop()

namespace nx::aio {

namespace {

mallocfree_memory default_memory;
memory_proxy default_allocator(&default_memory);

rid next_rid = 0;

struct idle_item {
    rid id;
    uv_idle_t uv_obj;
    idle_handle callback;
    list_head list;
};

NX_LIST_HEAD(idles);

} // namespace

void run(int type)
{
    switch (type) {
        case wait_all: {
            uv_run(LOOP, UV_RUN_DEFAULT);
            break;
        }
        case wait_once: {
            uv_run(LOOP, UV_RUN_ONCE);
            break;
        }
        case no_wait: {
            uv_run(LOOP, UV_RUN_NOWAIT);
            break;
        }
    }
}

void set_allocator(memory_proxy allocator) { default_allocator = allocator; }

static void idle_callback(uv_idle_t* handle)
{
    auto* idle = reinterpret_cast<idle_item*>(handle->data);
    idle->callback(idle->id);
}

rid create_idle(const idle_handle& handler)
{
    NX_LOG_D("aio create_idle");

    auto* idle = (idle_item*)default_allocator.allocate(sizeof(idle_item));
    idle->id = next_rid++;
    idle->callback = handler;
    list_add_tail(&(idle->list), &idles);
    uv_idle_init(LOOP, &(idle->uv_obj));
    idle->uv_obj.data = idle;
    uv_idle_start(&(idle->uv_obj), idle_callback);
    return idle->id;
}

static idle_item* find_idle_item(rid idle_id, list_head* head)
{
    idle_item* idle = nullptr;
    NX_LIST_FOR_EACH(ptr, head)
    {
        idle = NX_LIST_ENTRY(ptr, struct idle_item, list);
        if (idle->id == idle_id)
            return idle;
    }
    return nullptr;
}

void destroy_idle(rid idle_id)
{
    auto* idle = find_idle_item(idle_id, &idles);
    NX_ASSERT(idle, "idle_id isn't exist");

    uv_idle_stop(&(idle->uv_obj));
    uv_close(reinterpret_cast<uv_handle_t*>(&(idle->uv_obj)), nullptr);
    list_del(&(idle->list));
    default_allocator.free(idle);

    NX_LOG_D("aio destroy_idle");
}

} // namespace nx::aio