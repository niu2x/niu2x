#ifndef NX_LIST_H
#define NX_LIST_H

#include <stdlib.h>

#include <niu2x/api.h>
#include <niu2x/utils.h>

namespace nx {

struct list_t {
    struct list_t* prev;
    struct list_t* next;
};

inline bool list_empty(struct list_t* head) { return head->prev == head; }

inline void list_add(struct list_t* entry, struct list_t* head)
{
    head->next->prev = entry;
    entry->prev = head;

    entry->next = head->next;
    head->next = entry;
}

inline void list_add_tail(struct list_t* entry, struct list_t* head)
{
    head->prev->next = entry;
    entry->next = head;

    entry->prev = head->prev;
    head->prev = entry;
}

inline void list_del(struct list_t* entry)
{
    entry->prev->next = entry->next;
    entry->next->prev = entry->prev;
    entry->prev = nullptr;
    entry->next = nullptr;
}

inline void list_del_init(struct list_t* entry)
{
    entry->prev->next = entry->next;
    entry->next->prev = entry->prev;
    entry->prev = entry;
    entry->next = entry;
}

inline void list_init(struct list_t* entry)
{
    entry->prev = entry;
    entry->next = entry;
}

} // namespace nx

#define NX_LIST_INIT(name)                                                     \
    {                                                                          \
        &(name), &(name)                                                       \
    }
#define NX_LIST_DEFINE(name) struct list_t name = NX_LIST_INIT(name)

#define NX_LIST_ENTRY(ptr, clazz, field) NX_CONTAINER_OF(ptr, clazz, field)

#define NX_LIST_FOR_EACH(ptr, head)                                            \
    for (list_t* ptr = (head)->next; ptr != (head); ptr = ptr->next)

#endif /* NX_LIST_H */