#include "parse_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ParseTreeNode* createNode(const char* type, const char* value) {
    ParseTreeNode* node = (ParseTreeNode*)malloc(sizeof(ParseTreeNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed for node\n");
        return NULL;
    }
    
    node->type = type ? strdup(type) : NULL;
    node->value = value ? strdup(value) : NULL;
    node->children = NULL;
    node->num_children = 0;
    node->capacity = 0;
    
    return node;
}

void addChild(ParseTreeNode* parent, ParseTreeNode* child) {
    if (!parent || !child) return;
    
    if (parent->num_children >= parent->capacity) {
        int new_capacity = parent->capacity == 0 ? 4 : parent->capacity * 2;
        ParseTreeNode** new_children = (ParseTreeNode**)realloc(parent->children, new_capacity * sizeof(ParseTreeNode*));
        if (!new_children) {
            fprintf(stderr, "Memory allocation failed for children array\n");
            return;
        }
        parent->children = new_children;
        parent->capacity = new_capacity;
    }
    
    parent->children[parent->num_children++] = child;
}

void freeParseTree(ParseTreeNode* node) {
    if (!node) return;
    
    for (int i = 0; i < node->num_children; i++) {
        freeParseTree(node->children[i]);
    }
    
    free(node->type);
    free(node->value);
    free(node->children);
    free(node);
}

// Helper function to check if current token matches expected type and value
int match(token* tokens, int pos, int size, tokenType type, const char* value) {
    if (pos >= size) return 0;
    if (tokens[pos].type != type) return 0;
    if (value && strcmp(tokens[pos].value, value) != 0) return 0;
    return 1;
}

// Parse a factor (number, identifier, or parenthesized expression)
ParseTreeNode* parseFactor(token* tokens, int* pos, int size) {
    if (*pos >= size) {
        printf("End of tokens while parsing factor\n");
        return NULL;
    }
    
    ParseTreeNode* node = NULL;
    token current = tokens[*pos];
    
    if (current.type == number) {
        node = createNode("Number", current.value);
        (*pos)++;
    }
    else if (current.type == identifier) {
        node = createNode("Identifier", current.value);
        (*pos)++;
    }
    else if (current.type == separator && strcmp(current.value, "(") == 0) {
        (*pos)++; // consume '('
        node = createNode("Group", NULL);
        ParseTreeNode* expr = parseExpression(tokens, pos, size);
        if (expr) {
            addChild(node, expr);
        }
        if (*pos < size && tokens[*pos].type == separator && strcmp(tokens[*pos].value, ")") == 0) {
            (*pos)++; // consume ')'
        }
    }
    
    return node;
}

// Parse a term (factors separated by * or /)
ParseTreeNode* parseTerm(token* tokens, int* pos, int size) {
    ParseTreeNode* left = parseFactor(tokens, pos, size);
    if (!left) return NULL;
    
    while (*pos < size && tokens[*pos].type == operaTor && 
           (strcmp(tokens[*pos].value, "*") == 0 || strcmp(tokens[*pos].value, "/") == 0)) {
        const char* op = tokens[*pos].value;
        (*pos)++; // consume operator
        
        ParseTreeNode* right = parseFactor(tokens, pos, size);
        if (!right) break;
        
        ParseTreeNode* newNode = createNode("Operation", op);
        addChild(newNode, left);
        addChild(newNode, right);
        left = newNode;
    }
    
    return left;
}

// Parse an expression (terms separated by + or -)
ParseTreeNode* parseExpression(token* tokens, int* pos, int size) {
    ParseTreeNode* left = parseTerm(tokens, pos, size);
    if (!left) return NULL;
    
    while (*pos < size && tokens[*pos].type == operaTor && 
           (strcmp(tokens[*pos].value, "+") == 0 || strcmp(tokens[*pos].value, "-") == 0)) {
        const char* op = tokens[*pos].value;
        (*pos)++; // consume operator
        
        ParseTreeNode* right = parseTerm(tokens, pos, size);
        if (!right) break;
        
        ParseTreeNode* newNode = createNode("Operation", op);
        addChild(newNode, left);
        addChild(newNode, right);
        left = newNode;
    }
    
    return left;
}

// Parse a statement
ParseTreeNode* parseStatement(token* tokens, int* pos, int size) {
    if (*pos >= size) {
        printf("End of tokens while parsing statement\n");
        return NULL;
    }
    
    ParseTreeNode* node = NULL;
    token current = tokens[*pos];
    
    // Variable declaration
    if (current.type == keyword && 
        (strcmp(current.value, "int") == 0 || strcmp(current.value, "float") == 0 ||
         strcmp(current.value, "char") == 0 || strcmp(current.value, "_Bool") == 0)) {
        node = createNode("Declaration", current.value);
        (*pos)++; // consume type
        
        // Handle pointer types
        while (*pos < size && tokens[*pos].type == operaTor && strcmp(tokens[*pos].value, "*") == 0) {
            (*pos)++; // consume '*'
        }
        
        if (*pos < size && tokens[*pos].type == identifier) {
            addChild(node, createNode("Identifier", tokens[*pos].value));
            (*pos)++; // consume identifier
            
            // Handle array declarations
            if (*pos < size && tokens[*pos].type == separator && strcmp(tokens[*pos].value, "[") == 0) {
                (*pos)++; // consume '['
                ParseTreeNode* size_expr = parseExpression(tokens, pos, size);
                if (size_expr) {
                    addChild(node, size_expr);
                }
                if (*pos < size && tokens[*pos].type == separator && strcmp(tokens[*pos].value, "]") == 0) {
                    (*pos)++; // consume ']'
                }
            }
            
            // Handle initialization
            if (*pos < size && tokens[*pos].type == operaTor && strcmp(tokens[*pos].value, "=") == 0) {
                (*pos)++; // consume '='
                ParseTreeNode* expr = parseExpression(tokens, pos, size);
                if (expr) {
                    addChild(node, expr);
                }
            }
            
            if (*pos < size && tokens[*pos].type == separator && strcmp(tokens[*pos].value, ";") == 0) {
                (*pos)++; // consume ';'
            }
        }
    }
    
    return node;
}

// Parse the entire program
ParseTreeNode* parseProgram(token* tokens, int* pos, int size) {
    ParseTreeNode* root = createNode("Program", NULL);
    
    while (*pos < size) {
        ParseTreeNode* stmt = parseStatement(tokens, pos, size);
        if (stmt) {
            addChild(root, stmt);
        } else {
            (*pos)++; // Skip problematic token
        }
    }
    
    return root;
}

void printParseTree(ParseTreeNode* node, const char* prefix) {
    if (!node) return;
    
    printf("%s└─ %s", prefix, node->type);
    if (node->value) {
        printf(" (%s)", node->value);
    }
    printf("\n");
    
    for (int i = 0; i < node->num_children; i++) {
        int isLast = (i == node->num_children - 1);
        char* newPrefix = (char*)malloc(strlen(prefix) + 5);
        if (!newPrefix) return;
        sprintf(newPrefix, "%s%s", prefix, isLast ? "   " : "│  ");
        printParseTree(node->children[i], newPrefix);
        free(newPrefix);
    }
} 