#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic.h"
#include "symboltable.h"
#include "token.h"

/* Type information structure */
typedef struct {
    int type;           /* INT, FLOAT, or VOID */
    int is_array;       /* 1 if array, 0 if scalar */
    int array_size;     /* Size if array, 0 if scalar */
} TypeInfo;

/* Function information structure */
typedef struct {
    char *name;
    TypeInfo return_type;
    TypeInfo *param_types;
    int param_count;
} FunctionInfo;

static TypeInfo current_type;
static FunctionInfo *current_function = NULL;

/* Initialize semantic analyzer */
void init_semantic() {
    init_symbol_table();
}

/* Type checking for binary operations */
int check_binary_op(int op, TypeInfo left, TypeInfo right) {
    /* Arrays cannot be used in binary operations */
    if (left.is_array || right.is_array) {
        fprintf(stderr, "Semantic error: Arrays cannot be used in binary operations\n");
        return 0;
    }

    /* Void type cannot be used in operations */
    if (left.type == VOID || right.type == VOID) {
        fprintf(stderr, "Semantic error: Void type cannot be used in operations\n");
        return 0;
    }

    /* For now, we'll allow mixing of INT and FLOAT */
    return 1;
}

/* Check if an expression can be assigned to a variable */
int check_assignment(TypeInfo target, TypeInfo value) {
    /* Cannot assign to array without index */
    if (target.is_array && !value.is_array) {
        fprintf(stderr, "Semantic error: Cannot assign scalar to array\n");
        return 0;
    }

    /* Cannot assign array to scalar */
    if (!target.is_array && value.is_array) {
        fprintf(stderr, "Semantic error: Cannot assign array to scalar\n");
        return 0;
    }

    /* Cannot assign to or from void type */
    if (target.type == VOID || value.type == VOID) {
        fprintf(stderr, "Semantic error: Cannot assign void type\n");
        return 0;
    }

    /* Allow implicit conversion from int to float */
    if (target.type == FLOAT && value.type == INT) {
        return 1;
    }

    /* Types must match exactly */
    if (target.type != value.type) {
        fprintf(stderr, "Semantic error: Type mismatch in assignment\n");
        return 0;
    }

    return 1;
}

/* Check function call arguments */
int check_function_call(const char *func_name, TypeInfo *arg_types, int arg_count) {
    SymbolEntry *entry = lookup_symbol(func_name);
    if (!entry) {
        fprintf(stderr, "Semantic error: Undefined function '%s'\n", func_name);
        return 0;
    }

    FunctionInfo *func = (FunctionInfo *)entry;
    if (arg_count != func->param_count) {
        fprintf(stderr, "Semantic error: Wrong number of arguments for function '%s'\n", func_name);
        return 0;
    }

    for (int i = 0; i < arg_count; i++) {
        if (!check_assignment(func->param_types[i], arg_types[i])) {
            fprintf(stderr, "Semantic error: Argument type mismatch in function '%s'\n", func_name);
            return 0;
        }
    }

    return 1;
}

/* Set current type for declarations */
void set_current_type(int type, int is_array, int array_size) {
    current_type.type = type;
    current_type.is_array = is_array;
    current_type.array_size = array_size;
}

/* Enter function scope */
void enter_function(const char *name, int return_type) {
    current_function = (FunctionInfo *)malloc(sizeof(FunctionInfo));
    current_function->name = strdup(name);
    current_function->return_type.type = return_type;
    current_function->return_type.is_array = 0;
    current_function->param_types = NULL;
    current_function->param_count = 0;
    
    enter_scope();
}

/* Exit function scope */
void exit_function() {
    if (current_function) {
        free(current_function->name);
        free(current_function->param_types);
        free(current_function);
        current_function = NULL;
    }
    exit_scope();
}

/* Clean up semantic analyzer */
void cleanup_semantic() {
    if (current_function) {
        exit_function();
    }
    cleanup_symbol_table();
} 