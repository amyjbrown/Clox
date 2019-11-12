//Compiler.c
// Compiler implementation

#include <stdio.h>

#include "common.h"
#include "compiler.h"
#include "scanner.h"
#include "console.h"


void compile(const char* source){
    // Currently scanns through and outputs token 
    initScanner(source);
    int line = -1;
    
    for (;;) {
        //debugPrint("Compiling token...");

        Token token = scanToken();

        if (token.line != line) {
            printf("%4d ", token.line);
            line = token.line;
        } else {
            printf("   | ");
        }
        printf("%2d '%.*s'\n", token.type, token.length, token.start);

        if (token.type == TOKEN_EOF) break;
    }
}