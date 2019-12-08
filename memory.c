/* 
Memory.c
Implements memory.h and contents
*/
//#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "memory.h"
#include "object.h"

void* reallocate(void* previous, size_t oldSize, size_t newSize) {
    // If size is null, erase it and get null_pointer
    if (newSize == 0) {
        //printf("Freeing element...\n");
        free(previous);
        return NULL;
    }

    // reallocate the data into a new size
    // is Oldsize is zero :: malloc(data, newSize)
    //printf("Reallocating element....\n");
    return realloc(previous, newSize);
}

static void freeObject(Obj* object) {
    switch (object->type) {
        case OBJ_STRING: {
            ObjString* string = (ObjString*) object;
            FREE_ARRAY(char, string->chars, string->length+1);
            FREE(ObjString, object);
            break;
        }
    }
}

void freeObjects() {
    Obj* object = vm.objects;
    while (object != NULL) {
        Obj* next = object->next;
        freeObject(object);
        object = next;
    }
}