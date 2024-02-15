#include <stdbool.h>
#include <unistd.h>
#include <sys/mman.h>
#include <math.h>

#include "buddy.h"

void allocator_init(Allocator* allocator, int totalSize) {
    allocator->memory = (void*)mmap(NULL, totalSize, PROT_READ | PROT_WRITE, MAP_ANONYMOUS, -1, 0);
    for (int i = 0; i <= MAX_ORDER; i++) {
        allocator->free_lists[i] = NULL;
    }
    allocator->totalSize = totalSize;
    Node* initialBlock = (Node*)allocator->memory;
    initialBlock->next = NULL;
    allocator->free_lists[MAX_ORDER] = initialBlock;
}

void allocator_uninit(Allocator* allocator) {
    munmap(allocator->memory, allocator->totalSize);
}

void *balloc(Allocator *allocator, int size) {

    if (size == 0) {
        return NULL;
    }

    int order = ceil(log2(size));
    if (order > MAX_ORDER) {
        return NULL;
    }
    
    int i = order;
    while (i <= MAX_ORDER && allocator->free_lists[i] == NULL) {
        i++;
    }

    if (i > MAX_ORDER) {
        return NULL;
    }
    
    Node *allocatedBlock = allocator->free_lists[i];
    allocator->free_lists[i] = allocatedBlock->next;
    
    for (; i > order; i--) {
        Node* buddy = (Node*)((void*)allocatedBlock + (BLOCK_SIZE / (1 << i)));
        buddy->next = allocator->free_lists[i - 1];
        allocator->free_lists[i - 1] = buddy;
    }

    return allocatedBlock;
}

void bfree(Allocator *allocator, void *ptr, int size) {
    
    if (ptr == NULL || size == 0) {
        return;
    }

    int order = ceil(log2(size));

    Node* buddy = (Node*)ptr;
    for (int i = order; i < MAX_ORDER; i++) {
        int buddyOffset = ((void*)buddy - allocator->memory) ^ (BLOCK_SIZE / (1 << i));
        Node* potentialBuddy = (Node*)(allocator->memory + buddyOffset);
        if (potentialBuddy->next == allocator->free_lists[i] && potentialBuddy == buddy) {
            allocator->free_lists[i] = potentialBuddy->next;
            for (; i < order; i++) {
                Node* mergedBuddy = (Node*)((void*)buddy < (void*)potentialBuddy ? buddy : potentialBuddy);
                mergedBuddy->next = allocator->free_lists[i + 1];
                allocator->free_lists[i + 1] = mergedBuddy;
                buddy = mergedBuddy;
            }
            return;
        }
    }

    buddy->next = allocator->free_lists[order];
    allocator->free_lists[order] = buddy;
}