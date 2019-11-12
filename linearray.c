// Linearray.c
// Imeplementation details for Linearray.h

//#include "stdio.h"
#include <limits.h>
#include "linearray.h"
#include "memory.h"


// I will add Result types damnit

typedef struct {
    bool valid;
    int line;
} ResultLine;

//Get the line count of a line-array, given an index
int getLine(LineArray* array, int index) {
    // Iterate over
    int target = index;

    for (int i=0; i < array->count; i++) {
        // If we have yet to reach the goal, decrease the count
        if (array->lineCount[i] > target){
            return array->lines[i];
        }

        if (array->lineCount[i] <= target) {
            target -= array->lineCount[i];
        }

    }
    // If we haven't gotten here yet, return -2 as Error
    return INT_MIN;
}


//Initialize the ArrayList of lines
void initLineArray(LineArray* array) {
    //printf("Initializing Line Array...\n");

    array->capacity= 0;
    array->count = 0;
    array->lines = NULL;
    array->lineCount = NULL;
}

///Updates Array
void updateLineArray(LineArray *array, int line, int count) {
    //printf("Updating linearray with token at %d\n", line);

    // First, detect IF array already contains the line in it
    for (int index=0; index < array->count; array++) {
        if (array->lines[index] == line) {
                array->lineCount[index] += count;
                return;
            }
    }

    // Else, add line and lineCount to array, expanding the array if needed
    if (array->capacity < array->count + 1) {
        int old_capacity = array->capacity;
        array->capacity = GROW_CAPACITY(old_capacity);
        
        array->lines = GROW_ARRAY(
            array->lines, int, old_capacity, array->capacity);
        
        array->lineCount = GROW_ARRAY(
            array->lineCount, int, old_capacity, array->capacity);
    }
    
    // Note linecount is assigned `count` because it only now exists
    array->lines[array->count] = line;
    array->lineCount[array->count] = count;
}

//Free up the space of line array
void freeLineArray(LineArray* array) {
    //printf("Freeing Linearray");
    FREE_ARRAY(int, array->lines, array->capacity);
    FREE_ARRAY(int, array->lineCount, array->capacity);
    initLineArray(array);
}