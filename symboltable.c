#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symboltable.h"

#define TABLE_SIZE 211

/* Symbol table entry structure */
typedef struct SymbolEntry {
    char *name;
    int token_type;
    int scope;
    struct SymbolEntry *next;
} SymbolEntry;

/* Symbol table structure */
static SymbolEntry *hash_table[TABLE_SIZE];
static int current_scope = 0;

/* Hash function */
static unsigned int hash(const char *str) {
    unsigned int hash = 0;
    while (*str) {
        hash = (hash * 31 + *str) % TABLE_SIZE;
        str++;
    }
    return hash;
}

/* Initialize symbol table */
void init_symbol_table() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        hash_table[i] = NULL;
    }
}

/* Insert symbol into table */
void insert_symbol(const char *name, int token_type) {
    unsigned int index = hash(name);
    
    /* Check if symbol already exists in current scope */
    SymbolEntry *entry = hash_table[index];
    while (entry != NULL) {
        if (strcmp(entry->name, name) == 0 && entry->scope == current_scope) {
            return; /* Symbol already exists in current scope */
        }
        entry = entry->next;
    }

    /* Create new entry */
    entry = (SymbolEntry *)malloc(sizeof(SymbolEntry));
    entry->name = strdup(name);
    entry->token_type = token_type;
    entry->scope = current_scope;
    entry->next = hash_table[index];
    hash_table[index] = entry;
}

/* Lookup symbol in table */
SymbolEntry *lookup_symbol(const char *name) {
    unsigned int index = hash(name);
    SymbolEntry *entry = hash_table[index];
    
    /* Search in current and outer scopes */
    while (entry != NULL) {
        if (strcmp(entry->name, name) == 0 && entry->scope <= current_scope) {
            return entry;
        }
        entry = entry->next;
    }
    return NULL;
}

/* Enter new scope */
void enter_scope() {
    current_scope++;
}

/* Exit current scope */
void exit_scope() {
    if (current_scope > 0) {
        /* Remove all symbols in current scope */
        for (int i = 0; i < TABLE_SIZE; i++) {
            SymbolEntry *entry = hash_table[i];
            SymbolEntry *prev = NULL;
            
            while (entry != NULL) {
                if (entry->scope == current_scope) {
                    SymbolEntry *to_delete = entry;
                    if (prev == NULL) {
                        hash_table[i] = entry->next;
                    } else {
                        prev->next = entry->next;
                    }
                    entry = entry->next;
                    free(to_delete->name);
                    free(to_delete);
                } else {
                    prev = entry;
                    entry = entry->next;
                }
            }
        }
        current_scope--;
    }
}

/* Clean up symbol table */
void cleanup_symbol_table() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        SymbolEntry *entry = hash_table[i];
        while (entry != NULL) {
            SymbolEntry *next = entry->next;
            free(entry->name);
            free(entry);
            entry = next;
        }
        hash_table[i] = NULL;
    }
} 