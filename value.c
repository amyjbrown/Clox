// Value.c
// Contains implementation details for value.h
#include <stdio.h>

#include "memory.h"
#include "value.h"


void initValueArray(ValueArray* array) {
    array->values = NULL;
    array->capacity = 0;
    array -> count = 0;
}

void writeValueArray(ValueArray* array, Value value) {
    // Check to see if array would overgrow bounds
    if (array->capacity < array->count + 1) {
        // If yes, grow array and reallocate new array
        int old_capacity = array->capacity;
        array->capacity = GROW_CAPACITY(old_capacity);
        array->values = GROW_ARRAY(array->values, Value, old_capacity, array->capacity);
    }

    array->values[array->count] = value;
    array->count++;
}


void freeValueArray(ValueArray* array) {
    FREE_ARRAY(Value, array->values, array->capacity);
    initValueArray(array);
}

void printValue(Value value) {
    switch (value.type){
    case VAL_BOOL: printf(value.as.boolean ? "true" : "false");
        break;
    case VAL_NIL: printf("Nil");
        break;
    case VAL_NUMBER: printf("%g", value.as.number);
        break;
    }
}