%{
#include <stdio.h>
#include <string.h>
#include "token.h"
#include "symboltable.h"

int line_num = 1;
%}

/* Regular definitions */
DIGIT       [0-9]
LETTER      [a-zA-Z]
ID          {LETTER}({LETTER}|{DIGIT})*
NUMBER      {DIGIT}+(\.{DIGIT}+)?([Ee][+-]?{DIGIT}+)?
WHITESPACE  [ \t]+
NEWLINE     \n

%%

"if"        { return IF; }
"else"      { return ELSE; }
"while"     { return WHILE; }
"for"       { return FOR; }
"int"       { return INT; }
"float"     { return FLOAT; }
"void"      { return VOID; }
"return"    { return RETURN; }

{ID}        { 
    insert_symbol(yytext, ID);
    return ID;
}

{NUMBER}    {
    insert_symbol(yytext, NUMBER);
    return NUMBER;
}

"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return MULTIPLY; }
"/"         { return DIVIDE; }
"="         { return ASSIGN; }
"=="        { return EQ; }
"!="        { return NEQ; }
"<"         { return LT; }
"<="        { return LE; }
">"         { return GT; }
">="        { return GE; }

"("         { return LPAREN; }
")"         { return RPAREN; }
"{"         { return LBRACE; }
"}"         { return RBRACE; }
"["         { return LBRACKET; }
"]"         { return RBRACKET; }
";"         { return SEMICOLON; }
","         { return COMMA; }

{WHITESPACE} { /* Ignore whitespace */ }
{NEWLINE}    { line_num++; }

"/*"([^*]|\*+[^*/])*\*+"/" { /* Ignore multi-line comments */ }
"//".*      { /* Ignore single-line comments */ }

.           { printf("Lexical error on line %d: Unrecognized character %s\n", line_num, yytext); }

%%

int yywrap() {
    return 1;
} 