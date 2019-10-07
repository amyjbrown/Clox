// Linearray.c
// Imeplementation details for Linearray.h

//#include "stdio.h"
#include "linearray.h"
#include "memory.h"

//Get the line count of a line-array, given an index
int getLine(LineArray* array, int index) {
    // Iterate over
    int target = index;

    for (int i=0; i<array->count; i++) {
        // If we have yet to reach the goal, decrease the count
        if (array->lineCount[i] > target){
            return array->lines[i];
        }

        if (array->lineCount[i] <= target) {
            target -= array->lineCount[i];
        }

    }
    // If we haven't gotten here yet, return -1 as Error
    return -1;
}


//Initialize the ArrayList of lines
void initLineArray(LineArray* array) {
    //printf("Initializing Line Array...\n");

    array->capacity= 0;
    array->count = 0;
    array->lines = NULL;
    array->lineCount = NULL;
}

// Find and return the index that line has in the system
static int containsLine(LineArray* array, int line) {
    for (int i=0; i<array->count; i++) {
        if (array->lines[i] == line) return i;
    }

    //-1 for Does Not Contain
    return -1;
}

///Updates Array
void updateLineArray(LineArray* array, int line, int count) {
    //printf("Updating linearray with token at %d\n", line);

    // IF line in `lines` -- update it

    int index = containsLine(array, line);

    if (index != -1) { // IF Index is not Error Code
    // I think I fixed a bug here?
        //printf("Incrementing Line count");
        array->lineCount[index] += count;
        return;
    } 

    //ELSE -- add line to end (naturally appends); then increment
    else {
        //Check to see if array would be overgrown
        //printf("Expanding linearray arras for line %d\n", line);
        if (array->capacity < array->count + 1) {
            //IF yes, regrow array
            int old_capacity = array->capacity;
            array->capacity = GROW_CAPACITY(old_capacity);
            
            // Reallocate arrays.
            //printf("Reallocating array of line-indexes..\n");
            array->lines = GROW_ARRAY(array->lines, int, old_capacity,
                                        array->capacity);

            //printf("Reallocating array of line-counts..\n");
            array->lineCount = GROW_ARRAY(array->lineCount, int, old_capacity,
                                                array->capacity);
        }
        // Properly update the lines
        //printf("Now assigning new, proper values\n");
        array->lines[array->count] = line;
        array->lineCount[array->count] = count;
        array->count++;

        return;
    }
}

//Free up the space of line array
void freeLineArray(LineArray* array) {
    //printf("Freeing Linearray");
    FREE_ARRAY(int, array->lines, array->capacity);
    FREE_ARRAY(int, array->lineCount, array->capacity);
    initLineArray(array);
}