// Debug.c Implementation
/* 
Contents:
    * simpleInstruction():
        Prints name of and increments by 1 for Operators with no operands

*/
#include <stdio.h>
#include "debug.h"
#include "value.h"
// #include "linearray.h"

void disassembleChunk(Chunk* chunk, const char* name) {
    // debug
    // printf("Dissasembling Chunk...\n");
    printf ("chunk: count %d , capacity %d\n", chunk->count, chunk->capacity);

    printf("===== %s =====\n", name);
    
    for (int offset=0; offset < chunk->count;) {
        // Increment the offset by the number of operands we consumed
        offset = disassembleInstruction(chunk, offset);
    }
}

//Implementation specific
static int simpleInstruction(const char* name, int offset) {
    // printf("Dissasembling simple instruction...\n");

    printf("%s\n", name);
    return offset + 1;
}

static int constantInstruction(const char* name, Chunk* chunk, int offset) {
    uint8_t constant = chunk->code[offset+1];
    printf("%-16s %4d : \"", name, constant);
    printValue(chunk->constants.values[constant]);
    printf("\"\n");

    // Move forward Operator and Operand [2]
    return offset + 2;
}

int disassembleInstruction(Chunk* chunk, int offset) {
    //debug
    // printf("Dissassembling Instruction at %d\n", offset);
    printf("%04d ", offset);
    // Print line nmumber for now
    if (offset > 0 && 
    getLine(&chunk->lines, offset) == getLine(&chunk->lines, offset - 1) ){
        printf("   | ");
    } else {
        printf("%4d ", getLine(&chunk->lines, offset));
    }


    uint8_t instruction = chunk->code[offset];

    switch(instruction) {
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset); break;

        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset); break;
        
        case OP_NIL:
            return simpleInstruction("OP_NIL", offset);
        case OP_TRUE:
            return simpleInstruction("OP_TRUE", offset);
        case OP_FALSE:
            return simpleInstruction("OP_FALSE", offset);

        case OP_NEGATE:
            return simpleInstruction("OP_NEGATE", offset); break;
        case OP_NOT:
            return simpleInstruction("OP_NOT", offset); break;
            
        case OP_ADD:
            return simpleInstruction("OP_ADD", offset); break;
        case OP_SUBSTRACT:
            return simpleInstruction("OP_SUBTRACT", offset); break;
        case OP_MULPTIPLY:
            return simpleInstruction("OP_MULTIPLY", offset); break;
        case OP_DIVIDE:
            return simpleInstruction("OP_DIVIDE", offset); break;
        
        default:
            printf("Unknown opcode %d\n", instruction);
            return offset + 1; break;
    }
}

