#ifndef SEMANTIC_H
#define SEMANTIC_H

/* Type information structure */
typedef struct {
    int type;           /* INT, FLOAT, or VOID */
    int is_array;       /* 1 if array, 0 if scalar */
    int array_size;     /* Size if array, 0 if scalar */
} TypeInfo;

/* Function prototypes */
void init_semantic();
int check_binary_op(int op, TypeInfo left, TypeInfo right);
int check_assignment(TypeInfo target, TypeInfo value);
int check_function_call(const char *func_name, TypeInfo *arg_types, int arg_count);
void set_current_type(int type, int is_array, int array_size);
void enter_function(const char *name, int return_type);
void exit_function();
void cleanup_semantic();

#endif /* SEMANTIC_H */ 