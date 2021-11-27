#ifndef NX_HASHTAB_H
#define NX_HASHTAB_H

#include <string.h>

#include "niu2x/assert.h"
#include "niu2x/utils.h"
#include "niu2x/global.h"
#include "niu2x/list_head.h"

namespace nx {

struct hashtab_entry_t {
    struct list_head list;
    union {
        uint64_t u64;
    } key;
};

struct hashtab_t {
    struct hashtab_entry_t* entries;
    size_t capacity;
};

inline void hashtab_setup(struct hashtab_t* ht, size_t capacity)
{
    NX_ASSERT(capacity != 0, "invalid capacity");
    ht->entries = NX_ALLOC(struct hashtab_entry_t, capacity);
    ht->capacity = capacity;

    for (size_t i = 0; i < capacity; i++) {
        auto* head = &(ht->entries[i].list);
        head->prev = head;
        head->next = head;
    }
}

inline void hashtab_cleanup(struct hashtab_t* ht)
{
    NX_ASSERT(ht && ht->entries != nullptr && ht->capacity != 0, "");
    NX_FREE(ht->entries);
    ht->capacity = 0;
}

inline void __hashtab_set(
    struct hashtab_t* ht, uint64_t hash, struct hashtab_entry_t* entry)
{
    auto idx = hash % ht->capacity;
    auto* head = &(ht->entries[idx]);
    list_add(&(entry->list), &(head->list));
}

inline struct list_head* __hashtab_get(struct hashtab_t* ht, uint64_t hash)
{
    auto idx = hash % ht->capacity;
    auto* head = &(ht->entries[idx]);
    return &(head->list);
}

inline uint64_t hashtab_hash(uint64_t x)
{
    x = (x ^ (x >> 30)) * uint64_t(0xbf58476d1ce4e5b9);
    x = (x ^ (x >> 27)) * uint64_t(0x94d049bb133111eb);
    x = x ^ (x >> 31);
    return x;
}

inline void hashtab_set(
    struct hashtab_t* ht, uint64_t key, struct hashtab_entry_t* entry)
{
    __hashtab_set(ht, hashtab_hash(key), entry);
    entry->key.u64 = key;
}

inline struct hashtab_entry_t* hashtab_get(struct hashtab_t* ht, uint64_t key)
{
    struct list_head* head = __hashtab_get(ht, hashtab_hash(key));

    struct hashtab_entry_t* entry;
    NX_LIST_FOR_EACH(ptr, head)
    {
        entry = NX_LIST_ENTRY(ptr, struct hashtab_entry_t, list);
        if (entry->key.u64 == key)
            return entry;
    }
    return nullptr;
}

} // namespace nx

#define NX_HASHTAB_ENTRY(ptr, clazz, field)                                    \
    ((clazz*)(((uint8_t*)(ptr)) - NX_OFFSET_OF(clazz, field)))

#endif