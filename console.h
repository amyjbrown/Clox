/* Console.h
A console and command line managing interface, mostly used for printing out colors 

*/

#ifndef clox_console_h
#define clox_console_h

#include <stdio.h>

// #define DEBUG_FLAG_TRUE

const char* RED = "\033[0;31m";
const char* YELLOW = "\033[0;33m";
const char* GREEN = "\033[0;32m";
const char* RESET_COLOR = "\033[0;32m";

void debugPrint(const char* message) {
    #ifdef DEBUG_FLAG
    printf(GREEN);
    printf(message);
    printf("%s\n", RESET_COLOR)
    #endif
    return;
}


#endif