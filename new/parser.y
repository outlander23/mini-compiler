%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

void yyerror(const char *s);
int yylex(void);

%}

%union {
    int ival;
    char* sval;
}

%token <ival> NUMBER
%token <sval> ID
%token IF ELSE FOR WHILE
%token EQ NEQ LE GE LT GT
%token ASSIGN SEMICOLON
%token LPAREN RPAREN LBRACE RBRACE
%token PLUS MINUS TIMES DIVIDE

%type <ival> expr cond

%%

program:
    program statement
    | /* empty */
    ;

statement:
    expr SEMICOLON    { printf("Arithmetic result: %d\n", $1); }
    | ID ASSIGN expr SEMICOLON { set_var($1, $3); printf("Assignment: %s = %d\n", $1, $3); free($1); }
    | IF LPAREN cond RPAREN statement
        { printf("If statement: condition = %d\n", $3); if($3) ; }
    | IF LPAREN cond RPAREN statement ELSE statement
        { printf("If-Else: condition = %d\n", $3); if($3); else; }
    | WHILE LPAREN cond RPAREN statement
        {
            printf("While loop: condition initially = %d\n", $3);
            while($3) {
                /* Normally you'd execute $5 here */
                break; // Only demo, prevent infinite loop
            }
        }
    | FOR LPAREN opt_assign SEMICOLON cond SEMICOLON opt_assign RPAREN statement
        {
            printf("For loop: init; cond=%d; update\n", $5);
            for(; $5 ; ) {
                /* Normally you'd execute $9 here and update $7 */
                break; // Only demo, prevent infinite loop
            }
        }
    | LBRACE program RBRACE
    ;

opt_assign:
    /* empty */ { }
    | ID ASSIGN expr { set_var($1, $3); free($1); }
    ;

cond:
    expr EQ expr { $$ = ($1 == $3); }
    | expr NEQ expr { $$ = ($1 != $3); }
    | expr LE expr { $$ = ($1 <= $3); }
    | expr GE expr { $$ = ($1 >= $3); }
    | expr LT expr { $$ = ($1 < $3); }
    | expr GT expr { $$ = ($1 > $3); }
    | expr { $$ = ($1 != 0); }
    ;

expr:
    expr PLUS expr   { printf("Intermediate: %d + %d\n", $1, $3); $$ = $1 + $3; }
    | expr MINUS expr{ printf("Intermediate: %d - %d\n", $1, $3); $$ = $1 - $3; }
    | expr TIMES expr{ printf("Intermediate: %d * %d\n", $1, $3); $$ = $1 * $3; }
    | expr DIVIDE expr{ printf("Intermediate: %d / %d\n", $1, $3); $$ = $1 / $3; }
    | NUMBER         { $$ = $1; }
    | ID             { $$ = get_var($1); free($1); }
    | LPAREN expr RPAREN { $$ = $2; }
    ;

%%

void yyerror(const char *s) { fprintf(stderr, "Error: %s\n", s); }

int main() {
    printf("Enter statements. End each with a semicolon (;).\n");
    yyparse();
    return 0;
}