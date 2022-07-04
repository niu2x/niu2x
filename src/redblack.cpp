// #include <niu2x/redblack.h>
// #include <niu2x/utils.h>

// namespace nx {

// enum {
//     RED = 0,
//     BLACK = 1,
// };

// using rb_node_t = redblack_node_t;
// using rb_t = redblack_t;

// static void insereNo(rb_node_t** arvore, rb_node_t* pPai, rb_node_t** pMain,
//     rb_node_t* node, redblack_node_order_t order);
// static void consertaRB(rb_node_t** arvore, rb_node_t** pMain);

// static rb_node_t* tio(rb_node_t* arvore);

// static rb_node_t* avo(rb_node_t* arvore);
// static void rotacaoDireita(rb_node_t** no);
// static void dRotacaoDireita(rb_node_t** no);
// static void rotacaoEsquerda(rb_node_t** no);
// static void dRotacaoEsquerda(rb_node_t** no);

// static void dRotacaoEsquerda(rb_node_t** no)
// {
//     rotacaoDireita(&((*no)->right));
//     rotacaoEsquerda(no);
// }

// static void rotacaoEsquerda(rb_node_t** no)
// {

//     if (*no == nullptr)
//         return;

//     rb_node_t* aux = *no;
//     *no = aux->right;
//     aux->right = (*no)->left;
//     (*no)->left = aux;

//     (*no)->parent = aux->parent;
//     if (aux->left != nullptr)
//         (aux->left)->parent = aux;

//     aux->parent = *no;

//     aux->color = RED;
//     (*no)->color = BLACK;
// }

// static void dRotacaoDireita(rb_node_t** no)
// {
//     rotacaoEsquerda(&((*no)->left));
//     rotacaoDireita(no);
// }

// static void rotacaoDireita(rb_node_t** no)
// {
//     if (*no == nullptr)
//         return;

//     rb_node_t* aux = *no;
//     *no = aux->left;
//     aux->left = (*no)->right;
//     (*no)->right = aux;

//     (*no)->parent = aux->parent;
//     if (aux->right != nullptr)
//         (aux->right)->parent = aux;

//     aux->parent = *no;

//     aux->color = RED;
//     (*no)->color = BLACK;
// }

// static rb_node_t* avo(rb_node_t* arvore)
// {
//     if ((arvore != nullptr) && (arvore->parent != nullptr))
//         return (arvore->parent)->parent;
//     else
//         return nullptr;
// }

// static rb_node_t* tio(rb_node_t* arvore)
// {
//     rb_node_t* nAvo = avo(arvore);
//     if (nAvo == nullptr)
//         return nullptr;

//     if (arvore->parent == nAvo->left)
//         return nAvo->right;
//     else
//         return nAvo->left;
// }

// static void insereNo(rb_node_t** arvore, rb_node_t* pPai, rb_node_t** pMain,
//     rb_node_t* node, redblack_node_order_t order)
// {
//     if (*arvore == nullptr) {
//         *arvore = node;
//         (*arvore)->color = RED;

//         (*arvore)->left = nullptr;
//         (*arvore)->right = nullptr;
//         (*arvore)->parent = pPai;
//     } else {

//         int r = order(node, *arvore);

//         if (r < 0) {
//             insereNo(&((*arvore)->left), *arvore, pMain, node, order);
//             consertaRB(&((*arvore)->left), pMain);
//         } else if (r > 0) {
//             insereNo(&((*arvore)->right), *arvore, pMain, node, order);
//             consertaRB(&((*arvore)->right), pMain);
//         }
//     }

//     if (*arvore == *pMain)
//         (*arvore)->color = BLACK;
// }

// static void consertaRB(rb_node_t** arvore, rb_node_t** pMain)
// {

//     rb_node_t* nAvo = nullptr;
//     rb_node_t* nTio = nullptr;

//     if ((*arvore)->parent != nullptr) {
//         if ((*arvore)->parent->color == BLACK)
//             return;

//         if ((*arvore)->color == RED) {
//             nTio = tio(*arvore);
//             if (nTio != nullptr && nTio->color == RED) {
//                 nAvo = avo(*arvore);
//                 (*arvore)->parent->color = BLACK;
//                 nTio->color = BLACK;
//                 if (nAvo->parent != nullptr) {
//                     nAvo->color = RED;
//                 }
//             } else {
//                 nAvo = avo(*arvore);
//                 if (nAvo != nullptr) {
//                     if ((*arvore)->parent == nAvo->left) { // filho esquerdo
//                         if ((*arvore) == (nAvo->left)->left) {
//                             if (nAvo->parent != nullptr) {
//                                 if ((nAvo->parent)->left == nAvo)
//                                     rotacaoDireita(&((nAvo->parent)->left));
//                                 else
//                                     rotacaoDireita(&((nAvo->parent)->right));
//                             } else {
//                                 rotacaoDireita(pMain);
//                             }

//                         } else {
//                             if (nAvo->parent != nullptr) {
//                                 if ((nAvo->parent)->left == nAvo) {
//                                     dRotacaoDireita(&((nAvo->parent)->left));
//                                 } else
//                                     dRotacaoDireita(&((nAvo->parent)->right));
//                             } else {
//                                 dRotacaoDireita(pMain);
//                             }
//                         }
//                     } else {
//                         if ((*arvore) == ((nAvo->right)->right)) {
//                             if (nAvo->parent != nullptr) {
//                                 if (((nAvo->parent)->left) == nAvo) {
//                                     rotacaoEsquerda(&((nAvo->parent)->left));
//                                 } else
//                                     rotacaoEsquerda(&((nAvo->parent)->right));
//                             } else {
//                                 rotacaoEsquerda(pMain);
//                             }

//                         } else {
//                             if (nAvo->parent != nullptr) {
//                                 if ((nAvo->parent)->left == nAvo) {
//                                     dRotacaoEsquerda(&((nAvo->parent)->left));
//                                 } else {
//                                     dRotacaoEsquerda(&((nAvo->parent)->right));
//                                 }
//                             } else {
//                                 dRotacaoEsquerda(pMain);
//                             }
//                         }
//                     }
//                 }
//             }
//         }
//     }
// }

// void redblack_add(rb_t* tree, rb_node_t* node)
// {
//     insereNo(&(tree->root), nullptr, &(tree->root), node, tree->order);
// }

// void redblack_del(rb_t* tree, rb_node_t* node) { }

// } // namespace nx