//vm.c
/* Implementation of vm.h

*/

#include <stdio.h>
#include "compiler.h"
#include "common.h"
#include "vm.h"
#include "debug.h"

// Static Virtual Machine instance
VM vm;

static void resetStack() {
    // Because of array decomposition, this points to empty stack top
    vm.stackTop=vm.stack;
}

void initVM(){
    resetStack();
}


static InterpretResult run() {
    // ip points to next code to be operated 
    #define READ_BYTE() (*vm.ip++)
    #define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

    #define BINARY_OP(op) \
        do { \
            double b = pop(); \
            double a = pop(); \
            push(a op b); \
        } while (false)

    for (;;) {
        // Diagnostic tracing
        // Haha omg C array decomposition is something else
        #ifdef DEBUG_TRACE_EXECUTION
            printf("          ");
            for (Value* slot = vm.stack; slot < vm.stackTop; slot++) {
                printf("[");
                printValue(*slot);
                printf("]");
            }
            printf("\n");
            disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
        #endif


        uint8_t instruction;
        switch (instruction=READ_BYTE()) {
            case OP_RETURN: {
                return INTERPRET_OK;
                printValue(pop());
                printf("\n");
            }
        
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                push(constant);
                printf("\n");
                break;
            }

            case OP_NEGATE: push( -pop()); break;
            case OP_ADD: BINARY_OP(+); break;
            case OP_SUBSTRACT: BINARY_OP(-); break;
            case OP_MULPTIPLY: BINARY_OP(*); break;
            case OP_DIVIDE: BINARY_OP(/); break;

        }
    }

    #undef READ_BYTE
    #undef READ_CONSTANT
    #undef BINARY_OP
}


InterpretResult interpret(const char* source) {
    Chunk chunk;
    initChunk(&chunk);
    
    if (!compile(source, &chunk)) {
        freeChunk(&chunk);
        return INTERPRET_COMPILE_ERROR;
    }

    vm.chunk = &chunk;
    vm.ip = vm.chunk->code;

    InterpretResult result = run();

    freeChunk(&chunk);

    return result;
}


void freeVM(){}

void push(Value value) {
    *vm.stackTop = value;
    vm.stackTop++;
}

Value pop() {
    vm.stackTop --;
    return *vm.stackTop;
}