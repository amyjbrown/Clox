/* vm.h
Virtual Machine for interpreting bytecode chunks
Contents:
    *InterpretResult: enum
        Error and completion code for the interpreter
    *VM: struct
        Virtual Machine internal state, includes code and instruction pointer
    *initVM()
        Initial virtual machine
    *freeVM()
        Free up memory used by virtual machine
    * interpret()
        Run virtual machine and return it's value
*/

#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
}   InterpretResult;

/* 
Implementation Detials: ip Points to the code we're about to execute - hence at beginning it should point to zero
*/

typedef struct {
    Chunk* chunk;
    uint8_t* ip;
}   VM;

void initVM();
void freeVM();
InterpretResult interpret(Chunk* chunk);
#endif