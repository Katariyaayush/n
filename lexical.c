#include "lexical.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Convert token type to string for display
const char* tokenTypeToString(tokenType type) {
    switch(type) {
        case keyword: return "Keyword   ";
        case identifier: return "Identifier";
        case number: return "Number    ";
        case operaTor: return "Operator  ";
        case separator: return "Separator ";
        case string_literal: return "String    ";
        case comment: return "Comment   ";
        default: return "Unknown   ";
    }
}

// Check if a string is a C keyword
int isKeyword(const char* str) {
    static const char* keywords[] = {
        "auto", "break", "case", "char", "const", "continue", "default", "do",
        "double", "else", "enum", "extern", "float", "for", "goto", "if",
        "int", "long", "register", "return", "short", "signed", "sizeof", "static",
        "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while",
        "_Bool", "_Complex", "_Imaginary", "restrict", NULL
    };
    
    for (const char** k = keywords; *k != NULL; k++) {
        if (strcmp(str, *k) == 0) return 1;
    }
    return 0;
}

// Helper function to create a token
token createToken(tokenType type, const char* value, int line, int col) {
    token t;
    t.type = type;
    t.value = strdup(value);
    t.line = line;
    t.col = col;
    return t;
}

// Function to tokenize the input code
token* tokenize(const char* code, int* num_tokens) {
    int capacity = 100;
    token* tokens = (token*)malloc(capacity * sizeof(token));
    *num_tokens = 0;
    
    int line = 1;
    int col = 1;
    char buffer[1024];
    int buffer_pos = 0;
    
    for (int i = 0; code[i] != '\0'; i++) {
        char c = code[i];
        
        // Track line numbers
        if (c == '\n') {
            line++;
            col = 1;
            continue;
        }
        
        // Skip whitespace
        if (isspace(c)) {
            col++;
            continue;
        }
        
        // Handle string literals
        if (c == '"') {
            buffer_pos = 0;
            i++;
            col++;
            while (code[i] != '\0' && code[i] != '"') {
                buffer[buffer_pos++] = code[i];
                i++;
                col++;
            }
            buffer[buffer_pos] = '\0';
            
            if (code[i] == '"') {
                if (*num_tokens >= capacity) {
                    capacity *= 2;
                    token* new_tokens = (token*)realloc(tokens, capacity * sizeof(token));
                    if (!new_tokens) {
                        fprintf(stderr, "Memory allocation failed\n");
                        free(tokens);
                        return NULL;
                    }
                    tokens = new_tokens;
                }
                tokens[*num_tokens] = createToken(string_literal, buffer, line, col);
                (*num_tokens)++;
            }
            col++;
            continue;
        }
        
        // Handle comments
        if (c == '/' && code[i + 1] != '\0') {
            if (code[i + 1] == '/') {
                buffer_pos = 0;
                i += 2;
                col += 2;
                while (code[i] != '\0' && code[i] != '\n') {
                    buffer[buffer_pos++] = code[i];
                    i++;
                    col++;
                }
                buffer[buffer_pos] = '\0';
                i--;
                
                if (*num_tokens >= capacity) {
                    capacity *= 2;
                    token* new_tokens = (token*)realloc(tokens, capacity * sizeof(token));
                    if (!new_tokens) {
                        fprintf(stderr, "Memory allocation failed\n");
                        free(tokens);
                        return NULL;
                    }
                    tokens = new_tokens;
                }
                tokens[*num_tokens] = createToken(comment, buffer, line, col);
                (*num_tokens)++;
                continue;
            }
            else if (code[i + 1] == '*') {
                buffer_pos = 0;
                i += 2;
                col += 2;
                while (code[i] != '\0' && !(code[i] == '*' && code[i + 1] == '/')) {
                    if (code[i] == '\n') {
                        line++;
                        col = 1;
                    } else {
                        buffer[buffer_pos++] = code[i];
                        col++;
                    }
                    i++;
                }
                buffer[buffer_pos] = '\0';
                i += 2;
                col += 2;
                
                if (*num_tokens >= capacity) {
                    capacity *= 2;
                    token* new_tokens = (token*)realloc(tokens, capacity * sizeof(token));
                    if (!new_tokens) {
                        fprintf(stderr, "Memory allocation failed\n");
                        free(tokens);
                        return NULL;
                    }
                    tokens = new_tokens;
                }
                tokens[*num_tokens] = createToken(comment, buffer, line, col);
                (*num_tokens)++;
                continue;
            }
        }
        
        // Handle numbers
        if (isdigit(c)) {
            buffer_pos = 0;
            while (code[i] != '\0' && (isdigit(code[i]) || code[i] == '.')) {
                buffer[buffer_pos++] = code[i];
                i++;
                col++;
            }
            buffer[buffer_pos] = '\0';
            i--;
            
            if (*num_tokens >= capacity) {
                capacity *= 2;
                token* new_tokens = (token*)realloc(tokens, capacity * sizeof(token));
                if (!new_tokens) {
                    fprintf(stderr, "Memory allocation failed\n");
                    free(tokens);
                    return NULL;
                }
                tokens = new_tokens;
            }
            tokens[*num_tokens] = createToken(number, buffer, line, col);
            (*num_tokens)++;
            continue;
        }
        
        // Handle operators
        if (strchr("+-*/%=<>!&|^~", c)) {
            buffer[0] = c;
            buffer[1] = '\0';
            
            if (code[i + 1] != '\0') {
                char next = code[i + 1];
                if ((c == '+' && next == '+') || (c == '-' && next == '-') ||
                    (c == '=' && next == '=') || (c == '!' && next == '=') ||
                    (c == '<' && next == '=') || (c == '>' && next == '=') ||
                    (c == '&' && next == '&') || (c == '|' && next == '|')) {
                    buffer[1] = next;
                    buffer[2] = '\0';
                    i++;
                    col++;
                }
            }
            
            if (*num_tokens >= capacity) {
                capacity *= 2;
                token* new_tokens = (token*)realloc(tokens, capacity * sizeof(token));
                if (!new_tokens) {
                    fprintf(stderr, "Memory allocation failed\n");
                    free(tokens);
                    return NULL;
                }
                tokens = new_tokens;
            }
            tokens[*num_tokens] = createToken(operaTor, buffer, line, col);
            (*num_tokens)++;
            col++;
            continue;
        }
        
        // Handle separators
        if (strchr("(){}[];,", c)) {
            buffer[0] = c;
            buffer[1] = '\0';
            
            if (*num_tokens >= capacity) {
                capacity *= 2;
                token* new_tokens = (token*)realloc(tokens, capacity * sizeof(token));
                if (!new_tokens) {
                    fprintf(stderr, "Memory allocation failed\n");
                    free(tokens);
                    return NULL;
                }
                tokens = new_tokens;
            }
            tokens[*num_tokens] = createToken(separator, buffer, line, col);
            (*num_tokens)++;
            col++;
            continue;
        }
        
        // Handle identifiers and keywords
        if (isalpha(c) || c == '_') {
            buffer_pos = 0;
            while (code[i] != '\0' && (isalnum(code[i]) || code[i] == '_')) {
                buffer[buffer_pos++] = code[i];
                i++;
                col++;
            }
            buffer[buffer_pos] = '\0';
            i--;
            
            if (*num_tokens >= capacity) {
                capacity *= 2;
                token* new_tokens = (token*)realloc(tokens, capacity * sizeof(token));
                if (!new_tokens) {
                    fprintf(stderr, "Memory allocation failed\n");
                    free(tokens);
                    return NULL;
                }
                tokens = new_tokens;
            }
            
            // Check if it's a keyword
            if (isKeyword(buffer)) {
                tokens[*num_tokens] = createToken(keyword, buffer, line, col);
            } else {
                tokens[*num_tokens] = createToken(identifier, buffer, line, col);
            }
            (*num_tokens)++;
            continue;
        }
        
        col++;
    }
    
    return tokens;
}

void freeTokens(token* tokens, int num_tokens) {
    if (!tokens) return;
    
    for (int i = 0; i < num_tokens; i++) {
        free(tokens[i].value);
    }
    free(tokens);
} 