#include "header.h"

TNode *initNode() {
    TNode *node = (TNode*) malloc(sizeof(TNode));
    node->freq = 0;
    node->ch = '$';
    node->left = node->right = node->parent = NULL;

    return node;
}

MinHeap* initMinheap(int capacity) {
    MinHeap* heap = (MinHeap*) malloc (sizeof(MinHeap));
    heap->arr = (TNode**) malloc(capacity * sizeof(TNode*));

    heap->capacity = capacity;
    heap->size = 0;

    return heap;
}


int parent(int i) {
    return (i - 1) / 2;
}

int left_child(int i) {
    return (2*i + 1);
}

int right_child(int i) {
    return (2*i + 2);
}

TNode *get_min(MinHeap* heap) {
    return heap->arr[0];
}



MinHeap* insert_minheap(MinHeap* heap, char ch, unsigned int freq) {
    if (heap->size == heap->capacity) {
        fprintf(stderr, "Cannot insert %d. Heap is already full!\n", ch);
        return heap;
    }

    heap->arr[heap->size] = initNode();
    heap->arr[heap->size]->freq = freq;
    heap->arr[heap->size]->ch = ch;
    heap->size++;

    int curr = heap->size - 1;
    while (curr > 0 && heap->arr[parent(curr)]->freq > heap->arr[curr]->freq) {
        TNode *aux = heap->arr[parent(curr)];
        heap->arr[parent(curr)] = heap->arr[curr];
        heap->arr[curr] = aux;
       
        curr = parent(curr);
    }
    return heap; 
}

MinHeap* heapify(MinHeap* heap, int index) {
    if (heap->size <= 1)
        return heap;
    
    int left = left_child(index); 
    int right = right_child(index); 

    int smallest = index; 
    
    if (left < heap->size && heap->arr[left]->freq < heap->arr[index]->freq) 
        smallest = left; 
    
    
    if (right < heap->size && heap->arr[right]->freq < heap->arr[smallest]->freq) 
        smallest = right; 

    if (smallest != index) { 

        TNode *aux = heap->arr[index];
        heap->arr[index] = heap->arr[smallest];
        heap->arr[smallest] = aux;

        heap = heapify(heap, smallest); 
    }

    return heap;
}

TNode* extract_min(MinHeap* heap) {
    if (!heap || heap->size == 0)
        return NULL;

    TNode *node = heap->arr[0];
    
    heap->arr[0] = heap->arr[heap->size - 1];
    heap->size--;

    heap = heapify(heap, 0);
    return node;
}


void print_heap(MinHeap* heap) {
    printf("Min Heap:\n");
    for (int i = 0; i < heap->size; i++) {
        printf("%c %d -> ", heap->arr[i]->ch, heap->arr[i]->freq);
    }
    printf("\n");
}

void free_minheap(MinHeap* heap) {
    if (!heap)
        return;
    for (int i = 0; i < heap->size; i++) {
        free(heap->arr[i]);
    }
    free(heap->arr);
    free(heap);
}

void add_in_heap(MinHeap *heap, char ch) {
    int i;

    for (i = 0; i < heap->size; i++) {
        if (heap->arr[i]->ch == ch) {
            heap->arr[i]->freq++;
            heapify(heap, i);
            return;
        }
    }

    insert_minheap(heap, ch, 1);
}
