#ifndef NX_HASHTAB_H
#define NX_HASHTAB_H

#include <string.h>

#include "niu2x/assert.h"
#include "niu2x/utils.h"
#include "niu2x/global.h"
#include "niu2x/list_head.h"

namespace nx {

struct hashtab_t {
    struct list_head* entries;
    size_t capacity;
    size_t size;
};

struct hashtab_entry_t {
    struct list_head list;
    union {
        uint64_t u64;
    } key;

    struct hashtab_t* hashtab;
};

inline void hashtab_setup(struct hashtab_t* ht, size_t capacity = 32)
{
    NX_ASSERT(capacity != 0, "invalid capacity");
    ht->entries = NX_ALLOC(struct list_head, capacity);

    NX_ASSERT(ht->entries, "out of memory");
    ht->capacity = capacity;
    ht->size = 0;

    for (size_t i = 0; i < capacity; i++) {
        auto* head = &(ht->entries[i]);
        head->prev = head->next = head;
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
    list_add(&(entry->list), head);
}

inline struct list_head* __hashtab_get(struct hashtab_t* ht, uint64_t hash)
{
    auto idx = hash % ht->capacity;
    auto* head = &(ht->entries[idx]);
    return head;
}

inline uint64_t hashtab_hash(uint64_t x)
{
    x = (x ^ (x >> 30)) * uint64_t(0xbf58476d1ce4e5b9);
    x = (x ^ (x >> 27)) * uint64_t(0x94d049bb133111eb);
    x = x ^ (x >> 31);
    return x;
}


inline struct hashtab_entry_t* hashtab_get(struct hashtab_t* ht, uint64_t key)
{
    struct list_head* head = __hashtab_get(ht, hashtab_hash(key));

    struct hashtab_entry_t* entry;
    NX_LIST_FOR_EACH(ptr, head)
    {
        entry = NX_LIST_ENTRY(ptr, struct hashtab_entry_t, list);
        if (entry->key.u64 == key) {
            return entry;
        }
    }
    return nullptr;
}

inline void hashtab_del(struct hashtab_entry_t* entry)
{
    --entry->hashtab->size;
    entry->hashtab = nullptr;
    list_del(&(entry->list));
}

inline void hashtab_resize(struct hashtab_t* ht, size_t new_capacity);

inline void hashtab_set(
    struct hashtab_t* ht, uint64_t key, struct hashtab_entry_t* entry)
{
    auto* hh = hashtab_get(ht, key);
    if (hh)
        hashtab_del(hh);
    else {
        if (ht->size >= ht->capacity) {
            hashtab_resize(ht, (ht->size) << 1);
        }
    }

    __hashtab_set(ht, hashtab_hash(key), entry);

    entry->key.u64 = key;
    entry->hashtab = ht;
    ++ht->size;
}

inline void hashtab_resize(struct hashtab_t* ht, size_t new_capacity)
{
    struct hashtab_t new_ht;
    hashtab_setup(&new_ht, new_capacity);
    for (size_t idx = 0; idx < ht->capacity; idx++) {
        struct list_head* head = &(ht->entries[idx]);
        while (!list_empty(head)) {
            struct hashtab_entry_t* entry
                = NX_LIST_ENTRY(head->next, struct hashtab_entry_t, list);
            list_del(&(entry->list));
            hashtab_set(&new_ht, entry->key.u64, entry);
            entry->hashtab = ht;
        }
    }
    hashtab_cleanup(ht);
    *ht = new_ht;
}

} // namespace nx

#define NX_HASHTAB_ENTRY(ptr, clazz, field)                                    \
    ((clazz*)(((uint8_t*)(ptr)) - NX_OFFSET_OF(clazz, field)))

#endif