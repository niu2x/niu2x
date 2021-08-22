#ifndef NX_LIST_HEAD_H
#define NX_LIST_HEAD_H

#include <niu2x/api.h>

namespace nx {

struct API list_head {
    struct list_head* prev;
    struct list_head* next;
};

bool API list_empty(struct list_head* head);
void API list_add(struct list_head* entry, struct list_head* head);
void API list_add_tail(struct list_head* entry, struct list_head* head);
void API list_del(struct list_head* entry);
void API list_del_init(struct list_head* entry);

} // namespace nx

#define NX_LIST_HEAD_INIT(name)                                                \
    {                                                                          \
        &(name), &(name)                                                       \
    }
#define NX_LIST_HEAD(name) struct list_head name = LIST_HEAD_INIT(name)

#endif /* NX_LIST_HEAD_H */