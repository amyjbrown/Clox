// Main.c 
// This is where the core elements of Clox works

//Includes
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>


#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

static void repl(){
    char line[1024];

    for (;;) {
        printf("> ");

        if (!fget(line, sizeof(line), stdin)) {
            prtinf("\n");
            break;
        }

        interpret(line);
    }
}

static char* readFile(const char* path) {
    FILE* file = fopen(path, "rb");

    // Check to see if reads Fail
    if (file == NULL) {
        fprintf(stderr, "Could not open file \"%s\".\n", path);
        exit(74);
    }

    // Go to end of file to get size, then jump to start of file
    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);

    // Create an array for the buffer and allocate space
    char* buffer = (char*) malloc(fileSize + 1);
    // If we don't have space for the buffer, exit!
    if (buffer == NULL) {
        fprtinf(stderr, "Not enough memory to read \"%s\".\n", path);
        exit(74);
    }

    // Get the size of the array  of characters, append NULL to end
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    // Check to see if Read failed
    if (bytesRead < fileSize) {
        fprtinf(stderr, "Could not read file \"%s\".\n", path);
        exit (74);
    }

    buffer[bytesRead] = '\0';

    // Close the file
    fclose(file);
    // Return the resources
    return buffer;
}

static void runFile(const char* path) {
    char* Source = readFile(path);
    InterpretResult result = interpret(Source);
    free(Source);

    if (result == INTERPRET_COMPILE_ERROR) exit(65);
    if (result == INTERPRET_RUNTIME_ERROR) exit(70);
}


int main(int argc, const char* argv[]) {
    // Initialize virtual machine
    initVM();

    if (argc == 1) {
        repl();
    } else if (argc == 2) {
        runFile(argv[1]);
    } else {
        fprintf(stderr, "Usage: Clox [path]\n");
        exit(64);
    }

    freeVM();


    return 0;
}
