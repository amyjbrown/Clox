/*  Debug.h
Debugging and dissamebly Clox bytecode for programmer use

Contents:
    * dissasembleChunk(): 
        Takes a chunk and outputs human readable bytecode for the chunk
    *dissassembleInstruction():
        Print instruction name and increment the offset counter, iterating over the whole 
*/

#ifndef clox_debug_h
#define clox_debug_h

#include "chunk.h"

void disassembleChunk(Chunk* chunk, const char* name);
int disassembleInstruction(Chunk* chunk, int offset);

#endif