#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include <string.h>
#include "lexical.h"

typedef struct ParseTreeNode {
    char* type;        // Type of the node (e.g., "Statement", "Expression", etc.)
    char* value;       // Actual value/token
    struct ParseTreeNode** children;  // Array of child nodes
    int num_children;  // Number of children
    int capacity;      // Capacity of children array
} ParseTreeNode;

// Function declarations
ParseTreeNode* createNode(const char* type, const char* value);
void addChild(ParseTreeNode* parent, ParseTreeNode* child);
void freeParseTree(ParseTreeNode* node);

ParseTreeNode* parseProgram(token* tokens, int* pos, int size);
ParseTreeNode* parseStatement(token* tokens, int* pos, int size);
ParseTreeNode* parseExpression(token* tokens, int* pos, int size);
ParseTreeNode* parseTerm(token* tokens, int* pos, int size);
ParseTreeNode* parseFactor(token* tokens, int* pos, int size);
void printParseTree(ParseTreeNode* node, const char* prefix);

#endif // PARSE_TREE_H 