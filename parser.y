%{
#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "symboltable.h"

extern int yylex();
extern int line_num;
void yyerror(const char *s);
%}

%token IF ELSE WHILE FOR
%token INT FLOAT VOID
%token RETURN
%token ID NUMBER
%token PLUS MINUS MULTIPLY DIVIDE
%token ASSIGN EQ NEQ LT LE GT GE
%token LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET
%token SEMICOLON COMMA

%start program

%%

program
    : declaration_list
    ;

declaration_list
    : declaration
    | declaration_list declaration
    ;

declaration
    : var_declaration
    | fun_declaration
    ;

var_declaration
    : type_specifier ID SEMICOLON
    | type_specifier ID LBRACKET NUMBER RBRACKET SEMICOLON
    ;

type_specifier
    : INT
    | FLOAT
    | VOID
    ;

fun_declaration
    : type_specifier ID LPAREN params RPAREN compound_stmt
    ;

params
    : param_list
    | VOID
    ;

param_list
    : param
    | param_list COMMA param
    ;

param
    : type_specifier ID
    | type_specifier ID LBRACKET RBRACKET
    ;

compound_stmt
    : LBRACE local_declarations statement_list RBRACE
    ;

local_declarations
    : /* empty */
    | local_declarations var_declaration
    ;

statement_list
    : /* empty */
    | statement_list statement
    ;

statement
    : expression_stmt
    | compound_stmt
    | selection_stmt
    | iteration_stmt
    | return_stmt
    ;

expression_stmt
    : expression SEMICOLON
    | SEMICOLON
    ;

selection_stmt
    : IF LPAREN expression RPAREN statement
    | IF LPAREN expression RPAREN statement ELSE statement
    ;

iteration_stmt
    : WHILE LPAREN expression RPAREN statement
    | FOR LPAREN expression_stmt expression_stmt expression RPAREN statement
    ;

return_stmt
    : RETURN SEMICOLON
    | RETURN expression SEMICOLON
    ;

expression
    : var ASSIGN expression
    | simple_expression
    ;

var
    : ID
    | ID LBRACKET expression RBRACKET
    ;

simple_expression
    : additive_expression
    | additive_expression relop additive_expression
    ;

relop
    : LE
    | LT
    | GT
    | GE
    | EQ
    | NEQ
    ;

additive_expression
    : term
    | additive_expression addop term
    ;

addop
    : PLUS
    | MINUS
    ;

term
    : factor
    | term mulop factor
    ;

mulop
    : MULTIPLY
    | DIVIDE
    ;

factor
    : LPAREN expression RPAREN
    | var
    | NUMBER
    | ID LPAREN args RPAREN
    ;

args
    : /* empty */
    | arg_list
    ;

arg_list
    : expression
    | arg_list COMMA expression
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error on line %d: %s\n", line_num, s);
} 