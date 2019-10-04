/* Chunk.h
The code for the bytecode code segments and manipulating them
Contens:
    * OpCode: Enum for type of Bytecode
    * Chunk : A dynamic array-list contains pointers to to the codepoints
    * initChunk() initializes chunk memory
    * writeChunk() append bytecode to chunk array
    * freeChunk() Clears chunk from memory
    * addConstant() 
        Adds a constant Value(Lox bool or Lox number) to the stack and returns it's index for the Bytecode operator
*/
#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
    OP_RETURN,
    OP_CONSTANT,
}   OpCode;

// Dynamic Array Chunks
typedef struct {
    int count;
    int capacity;
    uint8_t* code;
    int* lines;
    ValueArray constants;
} Chunk;

void initChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
void freeChunk(Chunk* chunk);
int addConstant(Chunk* chunk, Value value);

#endif