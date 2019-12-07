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
};

struct sObjString{
    Obj obj;
    int length;
    char* chars;
};

ObjString* copyString(const char* chars, int length);

void printObject(Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_STRING:
            printf("%s", AS_CSTRING(value));
            break;
    }
}

// Inline to assist macro `IS_STRING(value)
static inline bool isObjType(Value value, ObjType type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif