#ifndef NX_REDBLACK_H
#define NX_REDBLACK_H

#include <cstdint>
#include <niu2x/api.h>

namespace nx {

struct redblack_node_t {
    redblack_node_t* parent;
    redblack_node_t* left;
    redblack_node_t* right;
    uint8_t color;
};

using redblack_node_order_t = int (*)(redblack_node_t*, redblack_node_t*);

struct redblack_t {
    redblack_node_t* root;
    redblack_node_order_t order;
};

NXAPI void redblack_add(redblack_t* tree, redblack_node_t* node);
NXAPI void redblack_del(redblack_t* tree, redblack_node_t* node);

} // namespace nx

#endif