// Scanner.h
// Lexical tokenizer and scanner for Clox

#ifndef clox_scanner_h
#define clox_scanner_h

//#undef DEBUG_FLAG

typedef enum {
    // single chars
    TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
    TOKEN_COMMA, TOKEN_DOT, TOKEN_MINUS, TOKEN_PLUS,
    TOKEN_SEMICOLON, TOKEN_SLASH, TOKEN_STAR,

    // semi-char tokens
    TOKEN_BANG, TOKEN_BANG_EQUAL,
    TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,
    TOKEN_GREATER, TOKEN_GREATER_EQUAL,
    TOKEN_LESS, TOKEN_LESS_EQUAL,

    // literals
    TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,

    // Keyword
    TOKEN_AND, TOKEN_CLASS, TOKEN_ELSE, TOKEN_FALSE,
    TOKEN_FOR, TOKEN_FUN, TOKEN_IF, TOKEN_NIL, TOKEN_OR,
    TOKEN_PRINT, TOKEN_RETURN, TOKEN_SUPER, TOKEN_THIS,
    TOKEN_TRUE, TOKEN_VAR, TOKEN_WHILE,

    // General special tokens
    TOKEN_ERROR,
    TOKEN_EOF,
    // TOKEN_REPL_BREAK
    /* todo for a future date
    TOKEN_UNFINISHED_STRING??
    TOKEN_SCOPE_UNFINISHED??
    */
} TokenType;


typedef struct {
    TokenType type;
    const char* start;
    int length;
    int line;
} Token;


void initScanner(const char* source);

Token scanToken();
#endif