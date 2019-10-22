// Console.c 
// Implemenation for console functions
// 

#include "console.h"

void debugPrint(const char* message) {
    #ifdef DEBUG_FLAG
    // va_list args;
    printf("%s%s\n", ITALIC_ON,GREEN);
    printf(message);
    printf("%s%s\n", ITALICS_OFF,RESET_COLOR);

    #endif
    return;
}

void errorPrint(const char* message) {
    #ifdef DEBUG_FLAG
    printf("%s\n", RED);
    printf(message);
    printf("%s\n", RESET_COLOR);
    #endif
}

void warningPrint(const char* message) {
    #ifdef DEBUG_FLAG
    printf("%s\n", YELLOW);
    printf(message);
    printf("%s\n", RESET_COLOR);
    #endif
}