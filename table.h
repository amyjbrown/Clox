// Table.h
// Interface for utilizing Hashtables

#ifndef clox_table_h
#define clox_table_h

#include "common.h"
#include "value.h"

//Hashmap individual entry representation
typedef struct {
    ObjString* key;
    Value value;
} Entry;

// Hashmap table structure
typedef struct {
    int count;
    int capacity;
    Entry* entries;
}   Table;

// Initialize table memory
void initTable(Table* table);
// Free and re-initializes table memory
void freeTable(Table* table);
// Checks to see if a table has a value, storing it in value if it does
bool tableGet(Table* table, ObjString* key, Value* value);
// Set element in a table with value
bool tableSet(Table* table, ObjString* key, Value value);
// Deletes the element from the table, returning if the operation successful
bool tableDelete(Table* table, ObjString* key);
// Copy all values from one table onto another
void tableAddAll(Table* from, Table* to);

#endif