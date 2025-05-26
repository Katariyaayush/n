#ifndef LEXICAL_H
#define LEXICAL_H

typedef enum {
    keyword,
    identifier,
    number,
    operaTor,
    separator,
    string_literal,
    comment,
    unknown
} tokenType;

typedef struct {
    tokenType type;
    char* value;
    int line;
    int col;
} token;

const char* tokenTypeToString(tokenType type);
token* tokenize(const char* code, int* num_tokens);
void freeTokens(token* tokens, int num_tokens);

#endif // LEXICAL_H 