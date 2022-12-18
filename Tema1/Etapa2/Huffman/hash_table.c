#include "header.h"

TNode **init_hashT() {
    TNode **v = (TNode**) calloc(300, sizeof(TNode*));

    return v;
}

void create_hashTable(TNode **arr, TNode *r) {
    if (r == NULL) {
        return;
    }

    if (r->left == NULL && r->right == NULL) {
        arr[r->ch] = r;
    }

    create_hashTable(arr, r->left);
    create_hashTable(arr, r->right);
}

Code get_code(TNode *node) {
    Code c;
    c.cod = 0;
    c.len = 0;

    while (node->parent != NULL) {
        if (node->parent->left == node) {
            c.len++;
        }

        if (node->parent->right == node) {
            c.cod = c.cod + (1 << c.len);
            c.len++;
        }

        node = node->parent;
    }

    return c;
}