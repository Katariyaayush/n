#ifndef TOKEN_H
#define TOKEN_H

/* Token types */
enum {
    /* Keywords */
    IF = 256,  /* Start at 256 to avoid conflicts with ASCII characters */
    ELSE,
    WHILE,
    FOR,
    INT,
    FLOAT,
    VOID,
    RETURN,

    /* Identifiers and literals */
    ID,
    NUMBER,

    /* Operators */
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    ASSIGN,
    EQ,     /* == */
    NEQ,    /* != */
    LT,     /* < */
    LE,     /* <= */
    GT,     /* > */
    GE,     /* >= */

    /* Delimiters */
    LPAREN,     /* ( */
    RPAREN,     /* ) */
    LBRACE,     /* { */
    RBRACE,     /* } */
    LBRACKET,   /* [ */
    RBRACKET,   /* ] */
    SEMICOLON,  /* ; */
    COMMA       /* , */
};

#endif /* TOKEN_H */ 