%{
#include "parser.tab.h"
#include <string.h>
%}

DIGIT      [0-9]
ID         [a-zA-Z_][a-zA-Z0-9_]*

%%

"if"        { return IF; }
"else"      { return ELSE; }
"for"       { return FOR; }
"while"     { return WHILE; }
{ID}        { yylval.sval = strdup(yytext); return ID; }
{DIGIT}+    { yylval.ival = atoi(yytext); return NUMBER; }
"=="        { return EQ; }
"!="        { return NEQ; }
"<="        { return LE; }
">="        { return GE; }
"<"         { return LT; }
">"         { return GT; }
"="         { return ASSIGN; }
";"         { return SEMICOLON; }
"("         { return LPAREN; }
")"         { return RPAREN; }
"{"         { return LBRACE; }
"}"         { return RBRACE; }
"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return TIMES; }
"/"         { return DIVIDE; }
[ \t\n\r]+  { /* skip whitespace */ }
.           { printf("Unknown character: %s\n", yytext); return 0; }

%%

int yywrap() { return 1; }