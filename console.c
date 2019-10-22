// Console.c 
// Implemenation for console functions
// 

#include "console.h"

void debugPrint(const char* message, ...) {
    #ifdef DEBUG_FLAG
    // Variadic iteration on *args
    va_list args;
    printf("%s%s\n", ITALIC_ON,GREEN);

    va_start(args, message);
    vprintf(message, args);
    va_end(args);

    printf("%s%s\n", ITALICS_OFF,RESET_COLOR);

    #endif
    return;
}

void errorPrint(const char* message, ...) {
    #ifdef DEBUG_FLAG
    va_list args;

    printf("%s\n", RED);

    va_start(args, message);
    vprintf(message, args);
    va_end(args);

    printf("%s\n", RESET_COLOR);


    #endif

}

void warningPrint(const char* message, ...) {
    #ifdef DEBUG_FLAG

    va_list args;

    printf("%s\n", YELLOW);
    
    va_start(args, message);
    vprintf(message, args);
    va_end(args);

    printf("%s\n", RESET_COLOR);
    #endif
}