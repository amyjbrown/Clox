//vm.c
/* Implementation of vm.h

*/

#include <stdio.h>
#include <stdarg.h>
#include "compiler.h"
#include "common.h"
#include "vm.h"
#include "debug.h"
#include "console.h"

// Static Virtual Machine instance
VM vm;

static void resetStack() {
    // Because of array decomposition, this points to empty stack top
    vm.stackTop=vm.stack;
}

static void runtimeError(const char* format, ...){

    size_t instruction = vm.ip - vm.chunk->code;
    int line = getLine(&vm.chunk->lines, (int) instruction);
    fprintf(stderr, "%s[line %d] in script ", RED, line);
    // Print the error
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "%s", RESET_COLOR);
    fputs("\n", stderr);


    resetStack();
}

void initVM(){
    resetStack();
}

static Value peek(int distance) {
    return vm.stackTop[-1-distance];
}

static bool isFalsey(Value value) {
    return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value) );
}


static InterpretResult run() {
    // ip points to next code to be operated 
    #define READ_BYTE() (*vm.ip++)
    #define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

    #define BINARY_OP(valueType, op)\
    do {\
      if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) {\
        runtimeError("Operands must be numbers.");\
        return INTERPRET_RUNTIME_ERROR;\
      }\
      double b = AS_NUMBER(pop()); \
      double a = AS_NUMBER(pop()); \
      push(valueType(a op b));\
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
                printValue(pop());
                printf("\n");
                return INTERPRET_OK;
            }
        
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                push(constant);
                // printf("\n");
                break;
            }

            case OP_EQUAL: {
                Value b = pop();
                Value a = pop();
                push(BOOL_VAL(valuesEqual(a, b)));
                break;
            }

            case OP_NIL: push(NIL_VAL); break;
            case OP_TRUE: push(BOOL_VAL(true)); break;
            case OP_FALSE: push(BOOL_VAL(false)); break;

            case OP_NOT:
                push(BOOL_VAL(isFalsey(pop())));
                break;
            case OP_NEGATE: 
                if (!IS_NUMBER(peek(0))) {
                    runtimeError("Operand must be a number.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                push(NUMBER_VAL(- AS_NUMBER(pop())));
                break;
            case OP_GREATER: BINARY_OP(BOOL_VAL, >);
            case OP_LESS:   BINARY_OP(BOOL_VAL, <);
            case OP_ADD: BINARY_OP(NUMBER_VAL, +); break;
            case OP_SUBSTRACT: BINARY_OP(NUMBER_VAL, -); break;
            case OP_MULPTIPLY: BINARY_OP(NUMBER_VAL, *); break;
            case OP_DIVIDE: BINARY_OP(NUMBER_VAL, /); break;

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

