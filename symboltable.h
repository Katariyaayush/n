#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

/* Symbol table entry structure forward declaration */
typedef struct SymbolEntry SymbolEntry;

/* Symbol table functions */
void init_symbol_table();
void insert_symbol(const char *name, int token_type);
SymbolEntry *lookup_symbol(const char *name);
void enter_scope();
void exit_scope();
void cleanup_symbol_table();

#endif /* SYMBOLTABLE_H */ 