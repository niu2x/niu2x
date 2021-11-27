#include <niu2x/test.h>

#include "niu2x/hashtab.h"

namespace nx::test {

void test_hashtab()
{

    struct hashtab_t ht;
    hashtab_setup(&ht, 1);

    struct data_t {
        struct hashtab_entry_t hash;
        int v;
    };

    for (int i = 0; i < 8  ; i++) {
        data_t* ptr = new data_t;
        ptr->v = i * i;
        hashtab_set(&ht, i, &(ptr->hash));
        printf("%d : %d %ld\n", i, ptr->v, ht.size);
    }

    for (int i = 0; i <   8; i++) {

        struct hashtab_entry_t* hh = hashtab_get(&ht, i);
        struct data_t* ptr = NX_HASHTAB_ENTRY(hh, struct data_t, hash);
        hashtab_del(hh);
        delete ptr;
        printf("%ld\n",  ht.size);
    }

    hashtab_cleanup(&ht);

    printf("test_hashtab: used: %lu\n", global::mem.used());
}

} // namespace nx::test