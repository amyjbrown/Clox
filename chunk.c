/* Chunk.c
Use this for the data structure of bytecode instruction sequences and manipulaitng them
Contenst:
    * initChunk(chunk*) initalize chunk state and properties
    * writeChunk(chunk*, byte) Write Byte to the Chunk and add stuff
    * 
*/


// Include
//#include <stdio.h> // For debugging
#include <stdlib.h>
#include "chunk.h"
#include "memory.h"

//Line-Encoding



void initChunk(Chunk* chunk) {
    //printf("Initializing chunk...\n");
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    // Initialized Dynamic Arrays
    initLineArray(&chunk->lines);
    initValueArray(&chunk->constants);
}

void writeChunk(Chunk* chunk, uint8_t byte, int line) {
    //printf("Writing Chunk with byte %d..\n", byte);
    // Check to see if ArrayList would be filled by operation
    // If yes: mutate the array so the capacity is twice times
    // And then copy the data to the mutated struct

    if (chunk->capacity < chunk->count + 1) {
        //printf("Rewriting chunk..\n");
        int old_capacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(old_capacity);

        chunk-> code = GROW_ARRAY(
            chunk->code, uint8_t, old_capacity, chunk->capacity
        );
        //printf("Rewrote chunk: Count %d, Capacity %d->%d \n",
        // chunk->count, old_capacity, chunk->capacity);

    }

    // Update the line count, *REGARDLESS* of if the code array has been expanded
    updateLineArray(&chunk->lines, line, 1);

    // Write the byte to the end of the ArrayList and update count
    chunk->code[chunk->count] = byte;
    chunk->count++;
}

void freeChunk(Chunk* chunk) {
    //printf("Freeing chunk...\n");
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    freeLineArray(&chunk->lines);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

//todo write descript
int addConstant(Chunk* chunk, Value value) {
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count -1;
}