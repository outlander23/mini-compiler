%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

extern int yylex();
extern int line_num;
extern char* yytext;
void yyerror(const char* s);
AST* program = NULL;
%}

%union {
    int intval;
    float floatval;
    char* strval;
    AST* ast;
}

/* Token definitions */
%token <strval> IDENTIFIER
%token <intval> INT_LITERAL
%token <floatval> FLOAT_LITERAL
%token IF ELSE FOR WHILE
%token INT FLOAT
%token PLUS MINUS MUL DIV MOD
%token ASSIGN
%token EQ NEQ LT LE GT GE
%token AND OR NOT
%token LPAREN RPAREN LBRACE RBRACE
%token SEMICOLON COMMA

/* Non-terminal types */
%type <ast> program stmt stmt_list expr term factor
%type <ast> if_stmt for_stmt while_stmt assign_stmt block
%type <ast> condition opt_expr

/* Operator precedence */
%left OR
%left AND
%left EQ NEQ
%left LT LE GT GE
%left PLUS MINUS
%left MUL DIV MOD
%right NOT
%right UMINUS
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%
program     : stmt_list                   { program = $1; }
            ;

stmt_list   : stmt                        { $$ = create_stmt_list_node(); add_statement($$, $1); }
            | stmt_list stmt              { add_statement($1, $2); $$ = $1; }
            ;

stmt        : assign_stmt                 { $$ = $1; }
            | if_stmt                     { $$ = $1; }
            | for_stmt                    { $$ = $1; }
            | while_stmt                  { $$ = $1; }
            | block                       { $$ = $1; }
            | expr SEMICOLON              { $$ = $1; }
            | SEMICOLON                   { $$ = NULL; }
            ;

assign_stmt : IDENTIFIER ASSIGN expr SEMICOLON
                                         { $$ = create_assign_node(create_identifier_node($1), $3); }
            ;

if_stmt     : IF LPAREN condition RPAREN stmt %prec LOWER_THAN_ELSE
                                         { $$ = create_if_node($3, $5, NULL); }
            | IF LPAREN condition RPAREN stmt ELSE stmt
                                         { $$ = create_if_node($3, $5, $7); }
            ;

opt_expr    : expr                       { $$ = $1; }
            | /* empty */                { $$ = NULL; }
            ;

for_stmt    : FOR LPAREN opt_expr SEMICOLON opt_expr SEMICOLON opt_expr RPAREN stmt
                                         { $$ = create_for_node($3, $5, $7, $9); }
            ;

while_stmt  : WHILE LPAREN condition RPAREN stmt
                                         { $$ = create_while_node($3, $5); }
            ;

block       : LBRACE stmt_list RBRACE    { $$ = $2; }
            | LBRACE RBRACE              { $$ = create_stmt_list_node(); }
            ;

condition   : expr                       { $$ = $1; }
            ;

expr        : expr PLUS term             { $$ = create_binary_expr_node(EXPR_PLUS, $1, $3); }
            | expr MINUS term            { $$ = create_binary_expr_node(EXPR_MINUS, $1, $3); }
            | term                       { $$ = $1; }
            | expr EQ expr               { $$ = create_binary_expr_node(EXPR_EQ, $1, $3); }
            | expr NEQ expr              { $$ = create_binary_expr_node(EXPR_NEQ, $1, $3); }
            | expr LT expr               { $$ = create_binary_expr_node(EXPR_LT, $1, $3); }
            | expr LE expr               { $$ = create_binary_expr_node(EXPR_LE, $1, $3); }
            | expr GT expr               { $$ = create_binary_expr_node(EXPR_GT, $1, $3); }
            | expr GE expr               { $$ = create_binary_expr_node(EXPR_GE, $1, $3); }
            | expr AND expr              { $$ = create_binary_expr_node(EXPR_AND, $1, $3); }
            | expr OR expr               { $$ = create_binary_expr_node(EXPR_OR, $1, $3); }
            | NOT expr                   { $$ = create_unary_expr_node(EXPR_NOT, $2); }
            | IDENTIFIER ASSIGN expr     { $$ = create_assign_node(create_identifier_node($1), $3); }
            ;

term        : term MUL factor            { $$ = create_binary_expr_node(EXPR_MUL, $1, $3); }
            | term DIV factor            { $$ = create_binary_expr_node(EXPR_DIV, $1, $3); }
            | term MOD factor            { $$ = create_binary_expr_node(EXPR_MOD, $1, $3); }
            | factor                     { $$ = $1; }
            ;

factor      : INT_LITERAL                { $$ = create_int_node($1); }
            | FLOAT_LITERAL              { $$ = create_float_node($1); }
            | IDENTIFIER                 { $$ = create_identifier_node($1); }
            | LPAREN expr RPAREN         { $$ = $2; }
            | MINUS factor %prec UMINUS  { $$ = create_unary_expr_node(EXPR_NEG, $2); }
            ;
%%

void yyerror(const char* s) {
    fprintf(stderr, "Error: %s at line %d: %s\n", s, line_num, yytext);
}

int main(int argc, char** argv) {
    printf("Mini Compiler for Arithmetic Expressions and Control Statements\n");
    printf("==========================================================\n\n");
    
    if (yyparse() == 0) {
        printf("\nParsing completed successfully!\n");
        
        if (program) {
            printf("\nGenerating Intermediate Code:\n");
            printf("============================\n");
            generate_intermediate_code(program);
            
            // Check if the program consists of a single arithmetic expression
            if (program->type == NODE_STMT_LIST && program->stmt_list.count == 1) {
                AST* stmt = program->stmt_list.statements[0];
                if (stmt && (stmt->type == NODE_BINARY_EXPR || stmt->type == NODE_UNARY_EXPR || 
                    stmt->type == NODE_INT_LITERAL || stmt->type == NODE_FLOAT_LITERAL)) {
                    printf("\nEvaluating Arithmetic Expression:\n");
                    printf("===============================\n");
                    
                    int result_int = eval_int_expr(stmt);
                    float result_float = eval_float_expr(stmt);
                    
                    // Determine if result should be int or float based on the expression
                    if (stmt->type == NODE_FLOAT_LITERAL || 
                        (stmt->type == NODE_BINARY_EXPR && stmt->expr_type == EXPR_DIV)) {
                        printf("Result: %f\n", result_float);
                    } else {
                        printf("Result: %d\n", result_int);
                    }
                }
            }
            
            // Clean up
            free_ast(program);
        }
    } else {
        printf("\nParsing failed!\n");
    }
    
    return 0;
}