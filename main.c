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
    writeChunk(&chunk, OP_RETURN, 123);
    
    // Testing and doing things
    disassembleChunk(&chunk, "test chunk");
    interpret(&chunk);

    freeVM();
    freeChunk(&chunk);


    return 0;
}
