#include <niu2x/test.h>

#include "niu2x/hashtab.h"

namespace nx::test {

void test_hashtab()
{

    struct hashtab_t ht;
    hashtab_setup(&ht, 1024 * 1024 * 2);

    struct data_t {
        struct hashtab_entry_t hash;
        int v;
    };

    for (int i = 0; i < 1024 * 1024; i++) {
        data_t* ptr = new data_t;
        ptr->v = i * i;
        hashtab_set(&ht, i, &(ptr->hash));
    }

    for (int i = 0; i < 1024 * 1024; i++) {
        struct hashtab_entry_t* hash = hashtab_get(&ht, i);
        struct data_t* ptr = NX_HASHTAB_ENTRY(hash, struct data_t, hash);
        delete ptr;
    }

    printf("test_hashtab: used: %lu\n", global::mem.used());
}

} // namespace nx::test