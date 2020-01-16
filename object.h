// Object.H

#ifndef clox_object_h
#define clox_object_h

#include "common.h"
#include "value.h"
// Macros for extracting type of macro
#define OBJ_TYPE(value)         (AS_OBJ(value)->type)

//Macro/inline for checking is a Value is of type Type
#define IS_STRING(value)         (isObjType(value, OBJ_STRING))
// Macro for extraing Lox and C representation of String
#define AS_STRING(value)        ( (ObjString*) AS_OBJ(value))
#define AS_CSTRING(value)       ( ((ObjString*) AS_OBJ(value)) -> chars)

typedef enum {
    OBJ_STRING
} ObjType;

struct sObj {
    ObjType type;
    struct sObj* next;
};
/*
* sObjString served as the implementation of ObjString - 
* It effaces a psuedo-GC linked list by checking which object
* a VM object will serve as the base root for the list
*/
struct sObjString{
    Obj obj;
    int length;
    char* chars;
    uint32_t hash;
};

// Allocate a String on the heap and return a reference to the created String-Object 
ObjString* takeString(char* chars, int length);
// Copy and allocate a string given a current string
ObjString* copyString(const char* chars, int length);
// Prints the value of an object
void printObject(Value value);
// Inline to assist macro `IS_STRING(value)
static inline bool isObjType(Value value, ObjType type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif