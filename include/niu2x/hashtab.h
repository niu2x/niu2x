#ifndef NX_HASHTAB_H
#define NX_HASHTAB_H

#include <string.h>

#include <cstdint>

#include <niu2x/api.h>
#include <niu2x/list.h>

namespace nx {

struct NXAPI hashtab_t {
    struct list_t* entries;
    size_t capacity;
    size_t size;
    struct list_t all;
};

struct NXAPI hashtab_entry_t {
    struct list_t list;
    struct list_t all;
    union {
        uint64_t u64;
    } key;

    struct hashtab_t* hashtab;
};

NXAPI void hashtab_setup(struct hashtab_t* ht, size_t capacity = 32);

NXAPI void hashtab_cleanup(struct hashtab_t* ht);

NXAPI inline void __hashtab_set(
    struct hashtab_t* ht, uint64_t hash, struct hashtab_entry_t* entry)
{
    auto idx = hash % ht->capacity;
    auto* head = &(ht->entries[idx]);
    list_add(&(entry->list), head);
    list_add(&(entry->all), &(ht->all));
}

NXAPI inline struct list_t* __hashtab_get(struct hashtab_t* ht, uint64_t hash)
{
    auto idx = hash % ht->capacity;
    auto* head = &(ht->entries[idx]);
    return head;
}

NXAPI inline uint64_t hashtab_hash(uint64_t x)
{
    x = (x ^ (x >> 30)) * uint64_t(0xbf58476d1ce4e5b9);
    x = (x ^ (x >> 27)) * uint64_t(0x94d049bb133111eb);
    x = x ^ (x >> 31);
    return x;
}

NXAPI inline struct hashtab_entry_t* hashtab_get(
    struct hashtab_t* ht, uint64_t key)
{
    struct list_t* head = __hashtab_get(ht, hashtab_hash(key));

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

NXAPI inline void hashtab_del(struct hashtab_entry_t* entry)
{
    --entry->hashtab->size;
    entry->hashtab = nullptr;
    list_del(&(entry->list));
    list_del(&(entry->all));
}

NXAPI inline void hashtab_resize(struct hashtab_t* ht, size_t new_capacity);

NXAPI inline void hashtab_set(
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

NXAPI inline void hashtab_resize(struct hashtab_t* ht, size_t new_capacity)
{
    struct hashtab_t new_ht;
    hashtab_setup(&new_ht, new_capacity);
    for (size_t idx = 0; idx < ht->capacity; idx++) {
        struct list_t* head = &(ht->entries[idx]);
        while (!list_empty(head)) {
            struct hashtab_entry_t* entry
                = NX_LIST_ENTRY(head->next, struct hashtab_entry_t, list);
            hashtab_del(entry);
            hashtab_set(&new_ht, entry->key.u64, entry);
            entry->hashtab = ht;
        }
    }
    hashtab_cleanup(ht);
    *ht = new_ht;
}

NXAPI inline struct hashtab_entry_t* hashtab_first(struct hashtab_t* ht)
{
    if (ht->size > 0) {
        return NX_LIST_ENTRY(ht->all.next, struct hashtab_entry_t, all);
    }
    return nullptr;
}

} // namespace nx

#define NX_HASHTAB_ENTRY(ptr, clazz, field)                                    \
    ((clazz*)(((uint8_t*)(ptr)) - NX_OFFSET_OF(clazz, field)))

#endif