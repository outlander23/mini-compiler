%{
#include "symtab.h"
#include "ast.h"
#include "codegen.h"
#include <stdio.h>
#include <stdlib.h>
%}

%token NUM ID
%token IF ELSE FOR WHILE
%token ASSIGN
%token PLUS MINUS TIMES DIVIDE
%token SEMICOLON
%token LPAREN RPAREN LBRACE RBRACE
%token LT GT LE GE EQ NE

%start program
%type <node> expr stmt program

%% 

program:
    stmt_list
;

stmt_list:
    stmt_list stmt
    | /* empty */
;

stmt:
    expr SEMICOLON {
        $$ = create_expr_node($1);
        generate_code($$);
    }
    | IF LPAREN expr RPAREN stmt ELSE stmt {
        $$ = create_if_node($3, $5, $7);
        generate_code($$);
    }
    | FOR LPAREN expr SEMICOLON expr SEMICOLON expr RPAREN stmt {
        $$ = create_for_node($3, $5, $7, $9);
        generate_code($$);
    }
    | WHILE LPAREN expr RPAREN stmt {
        $$ = create_while_node($3, $5);
        generate_code($$);
    }
;

expr:
    NUM {
        $$ = create_num_node($1);
    }
    | ID {
        $$ = create_id_node($1);
    }
    | expr PLUS expr {
        $$ = create_binop_node($1, $3, '+');
    }
    | expr MINUS expr {
        $$ = create_binop_node($1, $3, '-');
    }
    | expr TIMES expr {
        $$ = create_binop_node($1, $3, '*');
    }
    | expr DIVIDE expr {
        $$ = create_binop_node($1, $3, '/');
    }
    | LPAREN expr RPAREN {
        $$ = $2;
    }
;

%% 

int yyerror(char *s) {
    fprintf(stderr, "Error: %s\n", s);
    return 0;
}

int main(int argc, char **argv) {
    yyparse();
    return 0;
}