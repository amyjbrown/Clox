//vm.c
/* Implementation of vm.h

*/

#include <stdio.h>
#include "common.h"
#include "vm.h"

// Static Virtual Machine instance
VM vm;

void initVM(){}

InterpretResult interpret(Chunk* chunk) {
    vm.chunk = chunk;
    vm.ip = vm.chunk->code;
    return run();
}

static InterpretResult run() {
    // ip points to next code to be operated 
    #define READ_BYTE() (*vm.ip++)
    #define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

    for (;;) {
        uint8_t instruction;
        switch (instruction=READ_BYTE()) {
            case OP_RETURN: 
            {return INTERPRET_OK;}
        
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                printValue(constant);
                prtinf("\n");
                break;
            }

        }
    }

    #undef READ_BYTE
    #undef READ_CONSTANT
}


void freeVM(){}