#include <stdio.h>

#include "buddy.h"

int main(void) {

    Allocator allocator;
    size_t size = 1 << MAX_ORDER;

    allocator_init(&allocator, size);

    void *ptr1 = balloc(&allocator, 32);
    void *ptr2 = balloc(&allocator, 64);

    printf("%p\n", ptr1);
    printf("%p\n", ptr2);

    bfree(&allocator, ptr1, 32);
    bfree(&allocator, ptr2, 64);

    return 0;
}