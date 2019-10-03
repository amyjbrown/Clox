// Debug.c Implementation
/* 
Contents:
    * simpleInstruction():
        Prints name of and increments by 1 for Operators with no operands

*/
#include <stdio.h>
#include "debug.h"

void disassembleChunk(Chunk* chunk, const char* name) {
    // debug
    printf("Dissasembling Chunk...\n");
    printf ("chunk: count %d , capacity %d\n", chunk->count, chunk->capacity);

    printf("== %s ==\n", name);
    
    for (int offset=0; offset < chunk->count;) {
        // Increment the offset by the number of operands we consumed
        offset = disassembleInstruction(chunk, offset);
    }
}

static int simpleInstruction(const char* name, int offset) {
    // printf("Dissasembling simple instruction...\n");

    printf("%s\n", name);
    return offset + 1;
}


int disassembleInstruction(Chunk* chunk, int offset) {
    //debug
    printf("Dissassembling Instruction at %d\n", offset);

    printf("%03d ", offset);

    uint8_t instruction = chunk->code[offset];

    switch(instruction) {
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset); break;
        
        default:
            printf("Unknown opcode %d\n", instruction);
            return offset + 1; break;
    }
}

