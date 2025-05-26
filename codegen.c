#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"

/* Three-address code instruction types */
typedef enum {
    TAC_ADD,
    TAC_SUB,
    TAC_MUL,
    TAC_DIV,
    TAC_ASSIGN,
    TAC_LABEL,
    TAC_JUMP,
    TAC_JUMPZ,
    TAC_JUMPNZ,
    TAC_LT,
    TAC_LE,
    TAC_GT,
    TAC_GE,
    TAC_EQ,
    TAC_NEQ,
    TAC_PARAM,
    TAC_CALL,
    TAC_RETURN
} TACType;

/* Three-address code instruction structure */
typedef struct TACInstr {
    TACType op;
    char *dest;
    char *src1;
    char *src2;
    struct TACInstr *next;
} TACInstr;

/* Global variables */
static TACInstr *first_instr = NULL;
static TACInstr *last_instr = NULL;
static int temp_count = 0;
static int label_count = 0;

/* Create a new temporary variable name */
static char *new_temp() {
    char *temp = (char *)malloc(16);
    sprintf(temp, "t%d", temp_count++);
    return temp;
}

/* Create a new label name */
static char *new_label() {
    char *label = (char *)malloc(16);
    sprintf(label, "L%d", label_count++);
    return label;
}

/* Add a new three-address code instruction */
static void add_instr(TACType op, char *dest, char *src1, char *src2) {
    TACInstr *instr = (TACInstr *)malloc(sizeof(TACInstr));
    instr->op = op;
    instr->dest = dest ? strdup(dest) : NULL;
    instr->src1 = src1 ? strdup(src1) : NULL;
    instr->src2 = src2 ? strdup(src2) : NULL;
    instr->next = NULL;

    if (last_instr) {
        last_instr->next = instr;
    } else {
        first_instr = instr;
    }
    last_instr = instr;
}

/* Initialize code generator */
void init_codegen() {
    first_instr = NULL;
    last_instr = NULL;
    temp_count = 0;
    label_count = 0;
}

/* Generate code for binary operation */
char *gen_binary_op(char *left, char *op, char *right) {
    char *temp = new_temp();
    TACType tac_op;

    if (strcmp(op, "+") == 0) tac_op = TAC_ADD;
    else if (strcmp(op, "-") == 0) tac_op = TAC_SUB;
    else if (strcmp(op, "*") == 0) tac_op = TAC_MUL;
    else if (strcmp(op, "/") == 0) tac_op = TAC_DIV;
    else if (strcmp(op, "<") == 0) tac_op = TAC_LT;
    else if (strcmp(op, "<=") == 0) tac_op = TAC_LE;
    else if (strcmp(op, ">") == 0) tac_op = TAC_GT;
    else if (strcmp(op, ">=") == 0) tac_op = TAC_GE;
    else if (strcmp(op, "==") == 0) tac_op = TAC_EQ;
    else if (strcmp(op, "!=") == 0) tac_op = TAC_NEQ;
    else {
        fprintf(stderr, "Unknown operator: %s\n", op);
        return NULL;
    }

    add_instr(tac_op, temp, left, right);
    return temp;
}

/* Generate code for assignment */
void gen_assign(char *dest, char *src) {
    add_instr(TAC_ASSIGN, dest, src, NULL);
}

/* Generate code for if statement */
void gen_if(char *cond, char *true_label, char *false_label) {
    add_instr(TAC_JUMPZ, false_label, cond, NULL);
    add_instr(TAC_LABEL, true_label, NULL, NULL);
}

/* Generate code for while statement */
void gen_while(char *start_label, char *cond, char *end_label) {
    add_instr(TAC_LABEL, start_label, NULL, NULL);
    add_instr(TAC_JUMPZ, end_label, cond, NULL);
}

/* Generate code for function call */
char *gen_call(char *func_name, char **args, int arg_count) {
    char *temp = new_temp();
    
    /* Generate code for parameters */
    for (int i = 0; i < arg_count; i++) {
        add_instr(TAC_PARAM, NULL, args[i], NULL);
    }
    
    /* Generate call instruction */
    add_instr(TAC_CALL, temp, func_name, NULL);
    return temp;
}

/* Generate code for return statement */
void gen_return(char *value) {
    add_instr(TAC_RETURN, NULL, value, NULL);
}

/* Print generated code */
void print_code() {
    TACInstr *instr = first_instr;
    while (instr) {
        switch (instr->op) {
            case TAC_ADD:
                printf("%s = %s + %s\n", instr->dest, instr->src1, instr->src2);
                break;
            case TAC_SUB:
                printf("%s = %s - %s\n", instr->dest, instr->src1, instr->src2);
                break;
            case TAC_MUL:
                printf("%s = %s * %s\n", instr->dest, instr->src1, instr->src2);
                break;
            case TAC_DIV:
                printf("%s = %s / %s\n", instr->dest, instr->src1, instr->src2);
                break;
            case TAC_ASSIGN:
                printf("%s = %s\n", instr->dest, instr->src1);
                break;
            case TAC_LABEL:
                printf("%s:\n", instr->dest);
                break;
            case TAC_JUMP:
                printf("goto %s\n", instr->dest);
                break;
            case TAC_JUMPZ:
                printf("if %s == 0 goto %s\n", instr->src1, instr->dest);
                break;
            case TAC_JUMPNZ:
                printf("if %s != 0 goto %s\n", instr->src1, instr->dest);
                break;
            case TAC_PARAM:
                printf("param %s\n", instr->src1);
                break;
            case TAC_CALL:
                printf("%s = call %s\n", instr->dest, instr->src1);
                break;
            case TAC_RETURN:
                printf("return %s\n", instr->src1);
                break;
            default:
                printf("Unknown instruction\n");
        }
        instr = instr->next;
    }
}

/* Clean up code generator */
void cleanup_codegen() {
    TACInstr *instr = first_instr;
    while (instr) {
        TACInstr *next = instr->next;
        free(instr->dest);
        free(instr->src1);
        free(instr->src2);
        free(instr);
        instr = next;
    }
    first_instr = NULL;
    last_instr = NULL;
} 