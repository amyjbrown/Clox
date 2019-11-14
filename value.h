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

typedef enum {
    VAL_BOOL,
    VAL_NIL,
    VAL_NUMBER,
} ValueType;

typedef struct {
    ValueType type;
    union as
    {
        bool boolean;
        double number;
    };
    
} Value;

// Macros to check what the Lox valuetype is
#define IS_BOOL(value)      ((value).type == VAL_BOOL)
#define IS_NIL(value)       ((value).type == VAL_NIL)
#define IS_NUMBER(value)    ((value).type == VAL_NUMBER)

// Macro to extract C value equivalent to Lox value
#define AS_BOOL(value)      ((value).as.boolean)
#define AS_NUMBER(value)    ((value).as.number)

// Macros to generate Lox from C value with type
#define BOOL_VAL(value)     ((Value) { VAL_BOOL, {.boolean = (value)} })
#define NIL_VAL             ((Value) { VAL_NIL   {.number = 0}      })
#define NUMBER_VAL(value)   ((Value) { VAL_NUMBER, {.number = (value)}})


typedef struct {
    int capacity;
    int count;
    Value* values;
}   ValueArray;

void initValueArray(ValueArray* array);
void writeValueArray(ValueArray* array, Value value);
void freeValueArray(ValueArray* array);
void printValue(Value value);

#endif