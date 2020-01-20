//Compiler.c
// Compiler implementation

#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "compiler.h"
#include "scanner.h"
#include "console.h"
#include "debug.h"
#include "object.h"

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
    PREC_PRIMARY        // String, Number, Nil, Identifier, Groupping?
} Precedence;

// Function pointer for function that no arguements, returns no arguments
typedef void (*ParseFn) (bool canAssign);

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

    fprintf(stderr, "%s[line %d] Error ", RED, token->line);

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

//Expects 
static void consume(TokenType type, const char* message) {
    if (parser.current.type == type) {
        advance();
        return;
    }
}

static bool check(TokenType type) {
    return parser.current.type == type;
}

// Checks and advances if next token is of type Type
static bool match(TokenType type) {
    if (!check(type)) return false;
    advance();
    return true;
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
    // return 0;

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

static uint8_t identifierConstant(Token* name) {
    return makeConstant(
        OBJ_VAL(
            copyString(name->start, name->length)));
}

static uint8_t parseVariable(const char* errorMessage){
    consume(TOKEN_IDENTIFIER, errorMessage);
    return identifierConstant(&parser.previous);
}

static void defineVariable(uint8_t global) {
    emitBytes(OP_DEFINE_GLOBAL, global);
}

// Parses and emits bytecode for binary operator 
static void binary(bool canAssign) {
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
        // Logical
        case TOKEN_AND:     emitByte(OP_AND); break;
        case TOKEN_OR:      emitByte(OP_OR); break;
        // Comparison
        case TOKEN_BANG_EQUAL:  emitBytes(OP_EQUAL, OP_NOT); break;
        case TOKEN_EQUAL_EQUAL: emitByte(OP_EQUAL); break;
        case TOKEN_GREATER:     emitByte(OP_GREATER); break;
        case TOKEN_GREATER_EQUAL:   emitBytes(OP_LESS, OP_NOT); break;
        case TOKEN_LESS:      emitByte(OP_LESS); break;
        case TOKEN_LESS_EQUAL:  emitBytes(OP_GREATER, OP_NOT); break;

        default:
            debugPrint("Unable to parse binary operator.") ;return; // Unreachable
    }
}

// Produces and handles literals of Nil and Boolean nature
static void literal(bool canAssign) {
    switch (parser.previous.type)
    {
    case TOKEN_FALSE: emitByte(OP_FALSE);
        break;
    case TOKEN_NIL: emitByte(OP_NIL);
        break;
    case TOKEN_TRUE: emitByte(OP_TRUE);
        break;

    default:
        return; // Unreachable
    }
}

// Parses and emits bytecode for current expression inside parens
static void grouping(bool canAssign) {
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

// Functions that push around expressions

static void number(bool canAssign) {
    double value = strtod(parser.previous.start, NULL);
    emitConstant(NUMBER_VAL(value));
}

static void string(bool canAssign) {
    emitConstant(OBJ_VAL(
    copyString(
        parser.previous.start + 1,
        parser.previous.length - 2)
        )
    );
}

static void namedVariable(Token name, bool canAssign) {
    uint8_t arg = identifierConstant(&name);

    if (canAssign && match(TOKEN_EQUAL)) {
        expression();
        emitBytes(OP_SET_GLOBAL, arg);
    } else {
        emitBytes(OP_GET_GLOBAL, arg);
    }

    emitBytes(OP_GET_GLOBAL, arg);
}

static void variable(bool canAssign) {
    namedVariable(parser.previous, canAssign);
}

// Parses and emits bytecode for a unary expression
static void unary(bool canAssign) {
    TokenType operatorType = parser.previous.type;

    // Compile operand, since NEGATE follows the value
    parsePrecedence(PREC_UNARY);

    // Emit the operator instruction
    switch (operatorType) {
        case TOKEN_MINUS: emitByte(OP_NEGATE); break;
        case TOKEN_BANG: emitByte(OP_NOT); break;
        default:
            return; // Unreachable
    }
}

ParseRule rules[] = {
    /*  Null-denot  Left-denot  Precedence Ranking  Operator-Comment*/
    {   grouping,   NULL,       PREC_NONE},         // TOKEN_LEFT_PAREN
    {   NULL,       NULL,       PREC_NONE},         // TOKEN_RIGHT PAREN
    {   NULL,       NULL,       PREC_NONE},         // TOKEN_LEFT_BRACE
    {   NULL,       NULL,       PREC_NONE},         // TOKEN_RIGHT_BRACE
    {   NULL,       NULL,       PREC_NONE},         // TOKEN_COMMA
    {   NULL,       NULL,       PREC_NONE},         // TOKEN_DOT
    {   unary,      binary,     PREC_TERM},         // TOKEN_MINUS
    {   NULL,       binary,     PREC_TERM},         // TOKEN_PLUS
    {   NULL,       NULL,       PREC_NONE},         // TOKEN_SEMICOLON
    {   NULL,       binary,     PREC_FACTOR},       // TOKEN_SLASH
    {   NULL,       binary,     PREC_FACTOR},       // TOKEN_STAR
    {   unary,      NULL,       PREC_NONE},         // TOKEN_BANG
    {   NULL,       binary,     PREC_EQUALITY},     // TOKEN_BANG_EQUAL
    {   NULL,       NULL,       PREC_NONE},         // TOKEN_EQUAL
    {   NULL,       binary,     PREC_EQUALITY},     // TOKEN_EQUAL_EQUAL
    {   NULL,       binary,     PREC_COMPARISON},   // TOKEN_GREATER
    {   NULL,       binary,     PREC_COMPARISON},   // TOKEN_GREATER_EQUAL
    {   NULL,       binary,     PREC_COMPARISON},   // TOKEN_LESS
    {   NULL,       binary,     PREC_COMPARISON},   // TOKEN_LESS_EQUAL
    {   variable,       NULL,       PREC_NONE},         // TOKEN_IDENTIFIER
    {   string,     NULL,       PREC_NONE},         // TOKEN_STRING
    {   number,     NULL,       PREC_NONE},         // TOKEN_NUMBER
    {   NULL,       binary,     PREC_AND},          // TOKEN_AND
    {   NULL,       NULL,       PREC_NONE},         // TOKEN_CLASS
    {   NULL,       NULL,       PREC_NONE},         // TOKEN_ELSE
    {   literal,    NULL,       PREC_NONE},         // TOKEN_FALSE
    {   NULL,       NULL,       PREC_NONE},         // TOKEN_FOR
    {   NULL,       NULL,       PREC_NONE},         // TOKEN_FUN
    {   NULL,       NULL,       PREC_NONE},         // TOKEN_IF
    {   literal,    NULL,       PREC_NONE},         // TOKEN_NIL
    {   NULL,       binary,     PREC_OR},           // TOKEN_OR
    {   NULL,       NULL,       PREC_NONE},         // TOKEN_PRINT
    {   NULL,       NULL,       PREC_NONE},         // TOKEN_RETURN
    {   NULL,       NULL,       PREC_NONE},         // TOKEN_SUPER
    {   NULL,       NULL,       PREC_NONE},         // TOKEN_THIS
    {   literal,    NULL,       PREC_NONE},         // TOKEN_TRUE
    {   NULL,       NULL,       PREC_NONE},         // TOKEN_VAR
    {   NULL,       NULL,       PREC_NONE},         // TOKEN_WHILE
    {   NULL,       NULL,       PREC_NONE},         // TOKEN_ERROR
    {   NULL,       NULL,       PREC_NONE},         // TOKEN_EOF
};

static void parsePrecedence(Precedence precedence) {
    advance();

    ParseFn prefixRule = getRule(parser.previous.type)->prefix;
    if (prefixRule == NULL) {
        error("Expect expression");
        return;
    }

    bool canAssign = precedence <= PREC_ASSIGNMENT;
    prefixRule(canAssign);

    // prefixRule();

    while (precedence <= getRule(parser.current.type)->precedence) {
        advance();
        ParseFn infixRule = getRule(parser.previous.type)->infix;
        infixRule(canAssign);
    }
    if (canAssign && match(TOKEN_EQUAL)) {
        error("Invalid assignment target.");
    }
}

// Returns rule for token Type, using TokenTypess value as index
static ParseRule* getRule(TokenType type) {
    return &rules[type];
}

// Describe what precedences levels we're operator at
static void expression() {
    parsePrecedence(PREC_ASSIGNMENT);
}

static void varDeclaration() {
    uint8_t global = parseVariable("Expect variable name.");

    if (match(TOKEN_EQUAL)){
        expression();
    } else {
        emitByte(OP_NIL);
    }
    consume(TOKEN_SEMICOLON, "Expt ';' after variable delcaration");

    defineVariable(global);
}

static void expressionStatement() {
    expression();
    consume(TOKEN_SEMICOLON, "Expect ';' after expression.");
    emitByte(OP_POP);
}

static void printStatement() {
    expression();
    consume(TOKEN_SEMICOLON, "Expect ';' after value.");
    emitByte(OP_PRINT);
}

static void synchronize(){
    parser.panicMode = false;

    while (parser.current.type != TOKEN_EOF) {
        if (parser.previous.type==TOKEN_SEMICOLON) return;

        switch(parser.current.type){
            case TOKEN_CLASS:
            case TOKEN_FUN:
            case TOKEN_VAR:
            case TOKEN_FOR:
            case TOKEN_IF:
            case TOKEN_WHILE:
            case TOKEN_RETURN:
            case TOKEN_PRINT:
                return;
            default:
                0;
        }

        advance();
    }
}

static void statement() {
    if (match(TOKEN_PRINT)) {
        printStatement();
    } else {
        expressionStatement();
    }
}

static void declaration() {
    if (match(TOKEN_VAR)) {
        varDeclaration();
    } else {
    statement();
    }
    if (parser.panicMode) synchronize();
}

bool compile(const char* source, Chunk* chunk){
    // Currently scanns through and outputs token 
    initScanner(source);

    compilingChunk = chunk;

    parser.hadError = false;
    parser.panicMode = false;

    // int line = -1;
    advance();
    while (!match(TOKEN_EOF)) {
        declaration();
    }
    endCompiler();

    return ! parser.hadError;
}

