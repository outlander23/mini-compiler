%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "y.tab.h"

int line_num = 1;
%}

%option noyywrap

%%
[ \t]                   { /* ignore whitespace */ }
[\n]                    { line_num++; }
"if"                    { return IF; }
"else"                  { return ELSE; }
"for"                   { return FOR; }
"while"                 { return WHILE; }
"int"                   { return INT; }
"float"                 { return FLOAT; }
"+"                     { return PLUS; }
"-"                     { return MINUS; }
"*"                     { return MUL; }
"/"                     { return DIV; }
"%"                     { return MOD; }
"="                     { return ASSIGN; }
"=="                    { return EQ; }
"!="                    { return NEQ; }
"<"                     { return LT; }
"<="                    { return LE; }
">"                     { return GT; }
">="                    { return GE; }
"&&"                    { return AND; }
"||"                    { return OR; }
"!"                     { return NOT; }
"("                     { return LPAREN; }
")"                     { return RPAREN; }
"{"                     { return LBRACE; }
"}"                     { return RBRACE; }
";"                     { return SEMICOLON; }
","                     { return COMMA; }
[0-9]+                  { 
                          yylval.intval = atoi(yytext); 
                          return INT_LITERAL; 
                        }
[0-9]+\.[0-9]+          { 
                          yylval.floatval = atof(yytext); 
                          return FLOAT_LITERAL; 
                        }
[a-zA-Z][a-zA-Z0-9_]*   { 
                          yylval.strval = strdup(yytext);
                          return IDENTIFIER; 
                        }
.                       { 
                          printf("Lexical error: Unexpected character %s at line %d\n", yytext, line_num); 
                        }
%%