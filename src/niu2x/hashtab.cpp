#include <niu2x/hashtab.h>

#include "global.h"
#include "assert.h"

namespace nx {

void hashtab_cleanup(struct hashtab_t* ht)
{
    NX_ASSERT(ht && ht->entries != nullptr && ht->capacity != 0, "");
    NX_FREE(ht->entries);
    ht->capacity = 0;
    ht->size = 0;
    ht->all.prev = ht->all.next = &(ht->all);
}

void hashtab_setup(struct hashtab_t* ht, size_t capacity)
{
    NX_ASSERT(capacity != 0, "invalid capacity");
    ht->entries = NX_ALLOC(struct list_t, capacity);

    NX_ASSERT(ht->entries, "out of memory");
    ht->capacity = capacity;
    ht->size = 0;

    for (size_t i = 0; i < capacity; i++) {
        auto* head = &(ht->entries[i]);
        head->prev = head->next = head;
    }

    ht->all.prev = ht->all.next = &(ht->all);
}

} // namespace nx