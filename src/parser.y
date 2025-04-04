%{
#include <stdio.h>
#include <stdlib.h>
#define YYSTYPE int

extern int yylex(void);  // Declare the yylex function

void yyerror(const char *s);
%}

%token NUM
%token PLUS MINUS MUL DIV
%token LPAREN RPAREN SEMICOLON
%token IF ELSE FOR WHILE

%%

program:
    stmt_list
;

stmt_list:
    stmt
  | stmt_list stmt
;

stmt:
    expr SEMICOLON
        { printf("Arithmetic expression evaluated to: %d\n", $1); }
  | IF LPAREN expr RPAREN stmt
        { printf("If statement: condition evaluated to %d\n", $3); }
  | IF LPAREN expr RPAREN stmt ELSE stmt
        { printf("If-Else statement: condition evaluated to %d\n", $3); }
  | WHILE LPAREN expr RPAREN stmt
        { printf("While loop: condition evaluated to %d\n", $3); }
  | FOR LPAREN expr SEMICOLON expr SEMICOLON expr RPAREN stmt
        { printf("For loop: init=%d, condition=%d, update=%d\n", $3, $5, $7); }
;

expr:
    expr PLUS term
        { $$ = $1 + $3; printf("Intermediate Code: %d + %d = %d\n", $1, $3, $$); }
  | expr MINUS term
        { $$ = $1 - $3; printf("Intermediate Code: %d - %d = %d\n", $1, $3, $$); }
  | term
        { $$ = $1; }
;

term:
    term MUL factor
        { $$ = $1 * $3; printf("Intermediate Code: %d * %d = %d\n", $1, $3, $$); }
  | term DIV factor
        { $$ = $1 / $3; printf("Intermediate Code: %d / %d = %d\n", $1, $3, $$); }
  | factor
        { $$ = $1; }
;

factor:
    NUM
        { $$ = $1; }
  | LPAREN expr RPAREN
        { $$ = $2; }
;
%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main(void) {
    printf("Enter your statements (end each with a semicolon):\n");
    yyparse();
    return 0;
}