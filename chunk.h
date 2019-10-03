/* Chunk.h
The code for the bytecode code segments and manipulating them
Contens:
    * OpCode: Enum for type of Bytecode
    * Chunk : A dynamic array-list contains pointers to to the codepoints
    * initChunk() initializes chunk memory
    * writeChunk() append bytecode to chunk array
    * freeChunk() Clears chunk from memory
*/
#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"

typedef enum {
    OP_RETURN,
}   OpCode;

// Dynamic Array Chunks
typedef struct {
    int count;
    int capacity;
    uint8_t* code;
} Chunk;

void initChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte);
void freeChunk(Chunk* chunk);

#endif