#ifndef NX_LIST_HEAD_H
#define NX_LIST_HEAD_H

#include <stdlib.h>

#include <niu2x/api.h>

namespace nx {

struct API list_head {
    struct list_head* prev;
    struct list_head* next;
};

inline bool list_empty(struct list_head* head) { return head->prev == head; }

inline void list_add(struct list_head* entry, struct list_head* head)
{
    head->next->prev = entry;
    entry->prev = head;

    entry->next = head->next;
    head->next = entry;
}

inline void list_add_tail(struct list_head* entry, struct list_head* head)
{
    head->prev->next = entry;
    entry->next = head;

    entry->prev = head->prev;
    head->prev = entry;
}

inline void list_del(struct list_head* entry)
{
    entry->prev->next = entry->next;
    entry->next->prev = entry->prev;
    entry->prev = nullptr;
    entry->next = nullptr;
}

inline void list_del_init(struct list_head* entry)
{
    entry->prev->next = entry->next;
    entry->next->prev = entry->prev;
    entry->prev = entry;
    entry->next = entry;
}

} // namespace nx

#define NX_LIST_HEAD_INIT(name)                                                \
    {                                                                          \
        &(name), &(name)                                                       \
    }
#define NX_LIST_HEAD(name) struct list_head name = NX_LIST_HEAD_INIT(name)

#define NX_LIST_ENTRY(ptr, clazz, field)                                       \
    ((clazz*)(((uint8_t*)(ptr)) - NX_OFFSET_OF(clazz, field)))

#define NX_LIST_FOR_EACH(ptr, head)                                            \
    for (list_head* ptr = (head)->next; ptr != (head); ptr = ptr->next)

#endif /* NX_LIST_HEAD_H */