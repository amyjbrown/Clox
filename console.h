/* Console.h
A console and command line managing interface, mostly used for printing out colors and error messages

Contents:
    # Functions
* debugPrint(String message, ...)
    Prints to stdout a debug message if DEBUG_FLAG is defined, in special color, using printf style formatting
* errorPrint(String message, ...)
    Prints to stderr an error message using printf style formatting
*warningPrint(String message, ...)
    Prints to stdout a warning message in yellow using printf style formatting TODO: Have WARNING_FLAG implemented

    # Macros
* DEBUG_FLAG: Flag for debugPrint to output messages
* RED ANSI dark red color
* YELLOW ANSI dark yellow color
* GREEN ANSI dark greeen color
* RESET_COLOR ANSI code to use regular console color
* ITALIC_ON ANSI code to start using italics
* ITALICS_OFF ANSI code to stop using italics
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
void errorPrint(const char* message, ...);
void warningPrint(const char* message, ...);

#endif