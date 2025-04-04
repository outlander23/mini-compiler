%{
#include "y.tab.h"
%}

%option noyywrap

%%
[0-9]+          { yylval.num = atoi(yytext); return NUM; }
[+\-*/]        { return OP; }
"if"           { return IF; }
"else"         { return ELSE; }
"for"          { return FOR; }
"while"        { return WHILE; }
"("            { return LP; }
")"            { return RP; }
"{"            { return LB; }
"}"            { return RB; }
[ \t\n]        { /* ignore whitespace */ }
.              { return yytext[0]; }
%%