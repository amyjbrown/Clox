/* Console.h
A console and command line managing interface, mostly used for printing out colors 

Contents:
    * debugPrint(String message) outputs a message for debug purposes
*/

#ifndef clox_console_h
#define clox_console_h

// Includes
#include <stdio.h>
#include <stdarg.h>

#define DEBUG_FLAG

#define ITALIC_ON "\033[3m"
#define ITALICS_OFF "\033[23m"

#define RED "\033[0;31m"
#define YELLOW "\033[0;33m"
#define GREEN "\033[0;32m"
#define RESET_COLOR "\033[39;49m"

void debugPrint(const char* message, ...);
void errorPrint(const char* message);
void warningPrint(const char* message);

#endif