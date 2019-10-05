// Linecode.h
// Used for managing the runlength encoding of lines for debugging purposes
/* Contents:
    *struct LineArray: Dynamic Array for encoding line length
*/

#ifndef clox_linearray_h
#define clox_linearray_h

#include "common.h"

typedef struct {
    int count;
    int capacity;
    int* lines;
    int* lineCount;
}   LineArray;

/* 
What are our fundamental operations?
index    0   1   3   4
line:: [ 1,  2,  4, 10]
count: [ 3, 10,  5,  3]
* initialize the arraylist
* check if a line is included in it
* increment the lineCount of the line
* 
GOAL: a function getLine() that, when a chunk and bytecode index, will report the line it was on 
*/

int getLine(LineArray* array, int index);
void initLineArray(LineArray* array);
void updateLineArray(LineArray* array, int line, int count);
void freeLineArray(LineArray* array);

#endif