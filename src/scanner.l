%{
#include "y.tab.h"
#include <stdlib.h>
%}

%%

"if"           { return IF; }
"else"         { return ELSE; }
"for"          { return FOR; }
"while"        { return WHILE; }
"("            { return LPAREN; }
")"            { return RPAREN; }
";"            { return SEMICOLON; }
"+"            { return PLUS; }
"-"            { return MINUS; }
"*"            { return MUL; }
"/"            { return DIV; }
[0-9]+         { yylval = atoi(yytext); return NUM; }
[ \t\n]+       { /* skip whitespace */ }
.              { printf("Unexpected character: %s\n", yytext); }
%%

int yywrap() { return 1; }