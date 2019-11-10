//Compiler.c
// Compiler implementation

#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "compiler.h"
#include "scanner.h"
#include "console.h"
#include "debug.h"

typedef struct {
    Token current;
    Token previous;
    bool hadError;
    bool panicMode;
} Parser;


typedef enum {
    PREC_NONE, 
    PREC_ASSIGNMENT,    // =
    PREC_OR,            // or
    PREC_AND,           // and
    PREC_EQUALITY,      // == !=
    PREC_COMPARISON,    // < > <= >=
    PREC_TERM,          // + -
    PREC_FACTOR,        // * /
    PREC_UNARY,         // ! -
    PREC_CALL,          // . () []???
    PREC_PRIMARY
} Precedence;

// Function pointer for function that no arguements, returns no arguments
typedef void (*ParseFn) ();

typedef struct {
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
} ParseRule;

Parser parser;


Chunk* compilingChunk;

static Chunk* currentChunk() {
    return compilingChunk;
}

static void errorAt(Token* token, const char* message) {
    if (parser.panicMode) return;

    parser.panicMode = true;

    fprintf(stderr, "%s[line %d] Error", RED, token->line);

    if (token->type == TOKEN_EOF) {
        fprintf(stderr, " at end");
    } else if (token->type == TOKEN_ERROR) {
        // do nothing
    } else {
        fprintf(stderr, "at '%.*s'",token->length, token->start);
    }
    fprintf(stderr,  ": %s%s\n", message, RESET_COLOR);
    parser.hadError = true;
}

static void error(const char* message) {
    errorAt(&parser.previous, message);
}

static void errorAtCurrent(const char* message) {
    errorAt(&parser.current, message);
}

static void advance() {
    parser.previous = parser.current;

    for (;;) {
        parser.current = scanToken();
        if (parser.current.type != TOKEN_ERROR) break;

        errorAtCurrent(parser.current.start);
    }
}

static void consume(TokenType type, const char* message) {
    if (parser.current.type == type) {
        advance();
        return;
    }
}
// Writes Bytecode byte and metadata to current chunk
static void emitByte(uint8_t byte) {
    writeChunk(currentChunk(), byte, parser.previous.line);
}

// Writes two Bytecode values to current chunk
static void emitBytes(uint8_t byte1, uint8_t byte2) {
    emitByte(byte1);
    emitByte(byte2);
}

//Write Return Operator to current chunk
static void emitReturn() {
    emitByte(OP_RETURN);
}

// Add constant to chunk and returns bytecode operand for constant operator
static uint8_t makeConstant(Value value) {
    int constant = addConstant(currentChunk(), value);
    if (constant > UINT8_MAX) {
        error("Too many constants in one chunk.");
    }
    return 0;

    return (uint8_t) constant;
}

// Writes Constant Opertor and operand to to current chunk
static void emitConstant(Value value) {
    emitBytes(OP_CONSTANT, makeConstant(value));
}

// ends compiling
// Currently just emits Return opcode
static void endCompiler() {
    emitReturn();
    #ifdef DEBUG_PRINT_CODE
    if (!parser.hadError) {
        disassembleChunk(currentChunk(), "code");
    }
    #endif
}

// Forward delcarations

static void expression();
static ParseRule* getRule(TokenType type);
static void parsePrecedence(Precedence precedence);

// Parses and emits bytecode for binary operator 
static void binary() {
    // Remember the operator
    TokenType operatorType = parser.previous.type;

    // Compile right operand
    ParseRule* rule = getRule(operatorType);
    parsePrecedence( (Precedence)(rule->precedence + 1)  );

    // Emit the operator instruction.
    switch (operatorType) {
        case TOKEN_PLUS:    emitByte(OP_ADD); break;
        case TOKEN_MINUS:    emitByte(OP_SUBSTRACT); break;
        case TOKEN_STAR:    emitByte(OP_MULPTIPLY); break;
        case TOKEN_SLASH:   emitByte(OP_DIVIDE); break;

        default:
            debugPrint("Unable to parse binary operator.") ;return; // Unreachable
    }
}

// Parses and emits bytecode for current expression inside parens
static void grouping() {
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

// Functions that push around expressions

static void number() {
    double value = strtod(parser.previous.start, NULL);
    emitConstant(value);
}

// Parses and emits bytecode for a unary expression
static void unary() {
    TokenType operatorType = parser.previous.type;

    // Compile operand, since NEGATE follows the value
    parsePrecedence(PREC_UNARY);

    // Emit the operator instruction
    switch (operatorType) {
        case TOKEN_MINUS: emitByte(OP_NEGATE); break;
        default:
            return; // Unreachable
    }
}

ParseRule rules[] = {
    {grouping, NULL, PREC_NONE},    // TOKEN_LEFT_PAREN
    {NULL, NULL, PREC_NONE},        // TOKEN_RIGHT PAREN
    {NULL, NULL, PREC_NONE},        // TOKEN_LEFT_BRACE
    {NULL, NULL, PREC_NONE},        //TOKEN_RIGHT_BRACE
    {NULL, NULL, PREC_NONE},        // TOKEN_COMMA
    {NULL, NULL, PREC_NONE},        // TOKEN_DOT
    {unary, binary, PREC_TERM},     // TOKEN_MINUS
    {NULL, binary, PREC_TERM},      // TOKEN_PLUS
    {NULL, NULL, PREC_NONE},        // TOKEN_SEMICOLON
    {NULL, binary, PREC_FACTOR},    // TOKEN_SLASH
    {NULL, binary, PREC_FACTOR},    // TOKEN_STAR
    {NULL, NULL, PREC_NONE},        // TOKEN_BANG
    {NULL, NULL, PREC_NONE},        //TOKEN_BANG_EQUAL
    { NULL,     NULL,    PREC_NONE },// TOKEN_EQUAL
    {NULL, NULL, PREC_NONE},        // TOKEN_EQUAL_EQUAL
    {NULL, NULL, PREC_NONE},        // TOKEN_GREATER
    {NULL, NULL, PREC_NONE},        // TOKEN_GREATER_EQUAL
    {NULL, NULL, PREC_NONE},        // TOKEN_LESS
    {NULL, NULL, PREC_NONE},        // TOKEN_LESS_EQUAL
    {NULL, NULL, PREC_NONE},        // TOKEN_IDENTIFIER
    {NULL, NULL, PREC_NONE},        // TOKEN_STRING
    {number, NULL, PREC_NONE},      // TOKEN_NUMBER
    {NULL, NULL, PREC_NONE},        // TOKEN_AND
    {NULL, NULL, PREC_NONE},        // TOKEN_CLASS
    {NULL, NULL, PREC_NONE},        // TOKEN_ELSE
    {NULL, NULL, PREC_NONE},        // TOKEN_FALSE
    {NULL, NULL, PREC_NONE},        // TOKEN_FOR
    {NULL, NULL, PREC_NONE},        // TOKEN_FUN
    {NULL, NULL, PREC_NONE},        // TOKEN_IF
    {NULL, NULL, PREC_NONE},        // TOKEN_NIL
    {NULL, NULL, PREC_NONE},        // TOKEN_OR
    {NULL, NULL, PREC_NONE},        // TOKEN_PRINT
    {NULL, NULL, PREC_NONE},        // TOKEN_RETURN
    {NULL, NULL, PREC_NONE},        // TOKEN_SUPER
    {NULL, NULL, PREC_NONE},        // TOKEN_THIS
    {NULL, NULL, PREC_NONE},        // TOKEN_TRUE
    {NULL, NULL, PREC_NONE},        // TOKEN_VAR
    {NULL, NULL, PREC_NONE},        // TOKEN_WHILE
    {NULL, NULL, PREC_NONE},        // TOKEN_ERROR
    {NULL, NULL, PREC_NONE},        // TOKEN_EOF
};


static void parsePrecedence(Precedence precedence) {
    advance();

    ParseFn prefixRule = getRule(parser.previous.type)->prefix;
    if (prefixRule == NULL) {
        error("Expect expression");
        return;
    }

    prefixRule();

    while (precedence <= getRule(parser.current.type)->precedence) {
        advance();
        ParseFn infixRule = getRule(parser.previous.type)->infix;
        infixRule();
    }
}

// Returns rule for token Type, using TokenTypess value as index
static ParseRule* getRule(TokenType type) {
    return &rules[type];
}

// Describe what precedences levels we're operator at
void expression() {
    parsePrecedence(PREC_ASSIGNMENT);
}


bool compile(const char* source, Chunk* chunk){
    // Currently scanns through and outputs token 
    initScanner(source);

    compilingChunk = chunk;

    parser.hadError = false;
    parser.panicMode = false;

    int line = -1;
    advance();
    expression();
    consume(TOKEN_EOF, "Expect end of expression.");
    endCompiler();

    return ! parser.hadError;
}

