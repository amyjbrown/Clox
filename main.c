// Main.c 
// This is where the core elements of Clox works

//Includes
#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main(int argc, const char* argv[]) {
    // Initialize virtual machine
    initVM();

    Chunk chunk;
    initChunk(&chunk);
    //Psuedo chunk 
    int constant = addConstant(&chunk, 1.2);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123); 
    //more code
    constant = addConstant(&chunk, 3.4);    
    writeChunk(&chunk, OP_CONSTANT, 123);   
    writeChunk(&chunk, constant, 123);

    writeChunk(&chunk, OP_ADD, 123);        

    constant = addConstant(&chunk, 5.6);    
    writeChunk(&chunk, OP_CONSTANT, 123);   
    writeChunk(&chunk, constant, 123);      

    writeChunk(&chunk, OP_DIVIDE, 123);     


    writeChunk(&chunk, OP_NEGATE, 123);

    writeChunk(&chunk, OP_RETURN, 123);
    
    // Testing and doing things
    interpret(&chunk);
    disassembleChunk(&chunk, "test chunk");

    freeVM();
    freeChunk(&chunk);


    return 0;
}
