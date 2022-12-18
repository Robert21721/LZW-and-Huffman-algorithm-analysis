#include "header.h"

int is_leaf(TNode *r) {
    return (r->left == NULL) && (r->right == NULL);
}

void print_tree(TNode *r) {
    if(r == NULL) {
        return;
    }

    printf("%c %d\n", r->ch, r->freq);

    if (r->left) {
        print_tree(r->left);
    }

    if (r->right) {
        print_tree(r->right);
    }
}

TNode *createTree(FILE *fin, FILE *fout) {
    MinHeap *heap = initMinheap(10000);
    unsigned char ch;

    while (fread(&ch, sizeof(char), 1, fin)) {
        add_in_heap(heap, ch);
    }

    fwrite(&heap->size, sizeof(int), 1, fout);

    for (int i = 0; i < heap->size; i++) {
        fwrite(&heap->arr[i]->ch, sizeof(char), 1, fout);
        fwrite(&heap->arr[i]->freq, sizeof(unsigned int), 1, fout);
    }

    while (heap->size != 1) {
        TNode *left = extract_min(heap);
        TNode *right = extract_min(heap);

        TNode *p = initNode();
        p->freq = left->freq + right->freq;
        p->left = left;
        p->right = right;

        heap->arr[heap->size] = p;
        heap->size++;
        
        int curr = heap->size - 1;
        while (curr > 0 && heap->arr[parent(curr)]->freq > heap->arr[curr]->freq) {
            TNode *aux = heap->arr[parent(curr)];
            heap->arr[parent(curr)] = heap->arr[curr];
            heap->arr[curr] = aux;
        
            curr = parent(curr);
        }
    }

    return heap->arr[0];
}

void restore_parent_link(TNode *r) {
    if (r == NULL) {
        return;
    }

    if (r->left) {
        r->left->parent = r;
    }

    if (r->right) {
        r->right->parent = r;
    }

    restore_parent_link(r->left);
    restore_parent_link(r->right);
}
