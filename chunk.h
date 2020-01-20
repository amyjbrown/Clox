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
#include "linearray.h"


typedef enum {
    // Stateful operators
    OP_RETURN,          //For return statement
    OP_DEFINE_GLOBAL,   //For defining a global element
    OP_GET_GLOBAL,      //Pops name from stack and push value onto stack
    OP_SET_GLOBAL,      //Sets elements on stack
    OP_POP,             //Pops an operator off stack to ensure stack effect := 0
    OP_PRINT,           //Print a variable
    //Literals
    OP_CONSTANT,        //Pushses a constant onto the stack
    OP_NIL,             //Pushes a nil value onto the stack
    OP_TRUE,            //Pushes a true boolean onto the stack
    OP_FALSE,           //Pushes a false boolean onto the stack
    // Numeric operators
    OP_NEGATE,          //Pops constant and pushes it's numeric negation onto the stack
    OP_EQUAL,           //Pop two constants and compare them
    OP_GREATER,         //Pop two constants and compares them numerically to see if a > b
    OP_LESS,            //Pops two constants and compares them
    OP_ADD,             //Pops two constants, pushes sum onto stack
    OP_SUBSTRACT,       //Pops two constants, pushes difference onto stack
    OP_MULPTIPLY,       //Pops two constants, pushes their multiplication onto stack
    OP_DIVIDE,          //Pops two constants, pushes their dividend onto stack
    // Logical 
    OP_NOT,             //Pops constants and pushes it's logical negation onto stack
    OP_AND,             //Pops two constants, pushes logical `and` onto stack
    OP_OR,              //Pops two constants, pushes logical `or` onto stack
}   OpCode;

// Dynamic Array Chunks
typedef struct {
    int count;
    int capacity;
    uint8_t* code;
    // Implementation specific information
    LineArray lines;
    ValueArray constants;
} Chunk;

//Basic Chunk Manipulation
void initChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
void freeChunk(Chunk* chunk);

//Advanced
int addConstant(Chunk* chunk, Value value);

#endif