/* Value.h
Runtime value types
Contains:
    * Value: alias for Double, used for Lox numbers and bools
    * ValueArray: ArrayList containg Lox values
    * initValueArray(ValueArray*)
    * writeValueArray(ValueArray*, Value) append Value to 
    * freeValueArray(ValueArray*) Free up space and de-allocate ValueArray
    * printValue(Value) Print Value out for dissasembling

*/

#ifndef clox_value_h
#define clox_value_h

#include "common.h"

typedef double Value;

typedef struct {
    int capacity;
    int count;
    Value* values;
}   ValueArray;

void initValueArray(ValueArray* array);
void writeValueArray(ValueArray* array, Value value);
void freeValueArray(ValueArray* array);

#endif