#ifndef NX_OBJECT_POOL_H
#define NX_OBJECT_POOL_H

#include <cstdint>

#include <boost/noncopyable.hpp>

#include <niu2x/list.h>
#include <niu2x/object.h>

#include "global.h"
#include "niu2x/assert.h"

namespace nx {

template <class T, int INIT_CAPACITY = 256>
class obj_pool_t : private boost::noncopyable {
public:
    using elem_t = std::enable_if_t<std::is_base_of_v<object_t, T>, T>;
    // using obj_setup_t = void (*)(T* ptr);
    // using obj_cleanup_t = void (*)(T* ptr);

    using obj_freelist_t = freelist_t<elem_t, INIT_CAPACITY>;

    struct obj_freelist_entry_t {
        obj_freelist_t freelist;
        list_t list;
    };

    obj_pool_t(uint64_t capacity
        // , obj_setup_t obj_setup, obj_cleanup_t obj_cleanup
        )
    : capacity_(capacity)
    , size_(0)
    // , obj_setup_(obj_setup)
    // , obj_cleanup_(obj_cleanup)
    {
        list_init(&freelist_list_);
    }

    ~obj_pool_t() { clear(); }
    elem_t* alloc()
    {

        if (capacity_ && size_ == capacity_)
            return nullptr;

        elem_t* obj = nullptr;

        obj_freelist_entry_t* entry;
    __retry__:
        NX_LIST_FOR_EACH(ptr, &freelist_list_)
        {
            entry = NX_LIST_ENTRY(ptr, struct obj_freelist_entry_t, list);
            auto rid = entry->freelist.alloc();
            if (rid != obj_freelist_t::nil) {
                obj = entry->freelist.get(rid);
                obj->freelist = &(entry->freelist);
                obj->rid = rid;
                // obj_setup_(obj);
                ++size_;
                break;
            }
        }

        if (!obj) {
            entry = NX_NEW(struct obj_freelist_entry_t);
            list_add(&(entry->list), &freelist_list_);
            goto __retry__;
        }
        return obj;
    }
    void free(elem_t* ptr)
    {
        // obj_cleanup_(ptr);
        auto freelist = (obj_freelist_t*)(ptr->freelist);
        freelist->free(ptr->rid);
        --size_;
    }

    void clear()
    {
        NX_ASSERT_NO_THROW(size_ == 0, "some object in pool is not freed.");
        obj_freelist_entry_t* entry;
        while (!list_empty(&freelist_list_)) {
            entry = NX_LIST_ENTRY(
                freelist_list_.next, struct obj_freelist_entry_t, list);
            list_del(freelist_list_.next);
            NX_DELETE(entry);
        }
    }

private:
    list_t freelist_list_;
    uint64_t capacity_;
    uint64_t size_;
    // obj_setup_t obj_setup_;
    // obj_cleanup_t obj_cleanup_;
};

} // namespace nx

#endif

#define NX_OBJ_POOL_DECLARE(type, name) nx::obj_pool_t<type> name;
#define NX_OBJ_POOL_DEFINE(type, name, capacity)                               \
    nx::obj_pool_t<type> name((capacity));

#define NX_OBJ_POOL_DECLARE2(type) NX_OBJ_POOL_DECLARE(type##_t, type##_pool)
#define NX_OBJ_POOL_DEFINE2(type, capacity)                                    \
    NX_OBJ_POOL_DEFINE(type##_t, type##_pool, (capacity))