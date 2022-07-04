// #include "gtest/gtest.h"
// #include <niu2x/redblack.h>
// #include <niu2x/utils.h>

// struct stu_t {
//     int id;
//     nx::redblack_node_t rb;
// };

// int stu_rb_order(nx::redblack_node_t* a, nx::redblack_node_t* b)
// {
//     auto aa = NX_CONTAINER_OF(a, stu_t, rb);
//     auto bb = NX_CONTAINER_OF(b, stu_t, rb);
//     return aa->id - bb->id;
// }

// static void walk(nx::redblack_node_t* p)
// {
//     if (p) {
//         walk(p->left);

//         auto stu = NX_CONTAINER_OF(p, stu_t, rb);
//         printf("%d (%d, %d)\n", stu->id,
//             p->left ? NX_CONTAINER_OF(p->left, stu_t, rb)->id : -1,
//             p->right ? NX_CONTAINER_OF(p->right, stu_t, rb)->id : -1);

//         walk(p->right);
//     }
// }

// TEST(nx, redblack)
// {

//     stu_t stus[] = {
//         { 0, {} },
//         { 1, {} },
//         { 2, {} },
//         { 3, {} },
//         { 4, {} },
//         { 5, {} },
//         { 6, {} },
//         { 7, {} },
//         { 8, {} },
//         { 9, {} },
//     };

//     int nr_stus = sizeof(stus) / sizeof(stu_t);

//     nx::redblack_t tree { .root = nullptr, .order = stu_rb_order };
//     for (int i = 0; i < nr_stus; i++)
//         nx::redblack_add(&tree, &(stus[i].rb));

//     nx::redblack_node_t* p = tree.root;
//     walk(p);
// }
