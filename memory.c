/* 
Memory.c
Implements memory.h and contents
*/
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "memory.h"


void* reallocate(void* previous, size_t oldSize, size_t newSize) {
    printf("Reallocating value....\n");
    
    // If size is null, erase it and get null_pointer
    if (newSize == 0) {
        printf("Freeing element...\n");
        free(previous);
        return NULL;
    }

    // reallocate the data into a new size
    // is Oldsize is zero :: malloc(data, newSize)
    return realloc(previous, newSize);
}