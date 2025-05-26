#ifndef CODEGEN_H
#define CODEGEN_H

/* Function prototypes */
void init_codegen();
char *gen_binary_op(char *left, char *op, char *right);
void gen_assign(char *dest, char *src);
void gen_if(char *cond, char *true_label, char *false_label);
void gen_while(char *start_label, char *cond, char *end_label);
char *gen_call(char *func_name, char **args, int arg_count);
void gen_return(char *value);
void print_code();
void cleanup_codegen();

#endif /* CODEGEN_H */ 