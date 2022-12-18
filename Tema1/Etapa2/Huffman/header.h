#pragma once
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

typedef struct node{
    unsigned int freq;
    char ch;
    struct node *left;
    struct node *right;
    struct node *parent;
} TNode;

typedef struct MinHeap {
    TNode **arr;
    int size;
    int capacity;
} MinHeap;

typedef struct Code {
    unsigned int cod;
    int len;
} Code;


TNode *initNode();
MinHeap* initMinheap(int capacity);
int parent(int i);
int left_child(int i);
int right_child(int i);
TNode *get_min(MinHeap* heap);
MinHeap* insert_minheap(MinHeap* heap, char ch, unsigned int freq);
MinHeap* heapify(MinHeap* heap, int index);
TNode* extract_min(MinHeap* heap);
void print_heap(MinHeap* heap);
void free_minheap(MinHeap* heap);
void add_in_heap(MinHeap *heap, char ch);
void restore_parent_link(TNode *r);
TNode *createTree(FILE *fin, FILE *fout);
TNode **init_hashT();
void create_hashTable(TNode **arr, TNode *r);
Code get_code(TNode *node);
void print_tree(TNode *r);
void compression();
void decompression();
int is_leaf(TNode *r);