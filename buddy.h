#pragma once

#define MAX_ORDER 10
#define BLOCK_SIZE (1 << MAX_ORDER)

typedef struct Node {
    struct Node *next;
} Node;

typedef struct Allocator {
    Node *free_lists[MAX_ORDER + 1];
    void *memory;
    int totalSize;
} Allocator;

void allocator_init(Allocator *allocator, int totalSize);
void *balloc(Allocator *allocator, int size);
void bfree(Allocator *allocator, void *ptr, int size);