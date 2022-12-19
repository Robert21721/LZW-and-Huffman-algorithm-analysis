#include "header.h"

void decompression(char *file_name_in, char *file_name_out) {
    FILE *fin = fopen(file_name_in, "rb");
    FILE *fout = fopen(file_name_out, "wb");
    MinHeap *heap = initMinheap(1000);

    int nr_caracter_dif;
    fread(&nr_caracter_dif, sizeof(int), 1, fin);

    for (int i = 0; i < nr_caracter_dif; i++) {
        char ch;
        unsigned int freq;

        fread(&ch, sizeof(char), 1, fin);
        fread(&freq, sizeof(unsigned int), 1, fin);
        insert_minheap(heap, ch, freq);
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

    unsigned int buff;

    TNode *node = heap->arr[0];
    int pos = 8 * sizeof(int) - 1;
    while (fread(&buff, sizeof(unsigned int), 1, fin)) {
        for (int pos = 8 * sizeof(int) - 1; pos >= 0; pos--) {
            int dir = ((1 << pos) & buff) >> pos;

            if (dir == 0) {
                node = node->left;
            }

            if (dir == 1) {
                node = node->right;
            }

            if (node != NULL && is_leaf(node)) {
                node->freq--;
                if ((int)(node->freq) == -1) {
                    return;
                }

                fwrite(&node->ch, sizeof(char), 1, fout);
                
                node = heap->arr[0];
            }
        }
    }

}