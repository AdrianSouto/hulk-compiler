%{
#include "parser.hpp"
#include <cstdio>
#include <cstring>

int yylex();
%}

%option noyywrap

%%
[0-9]+  { yylval = atoi(yytext); return NUMBER; }
print   { return PRINT; }
\n      { return EOL; }
[ \t]   { /* ignore whitespace */ }
;       { return SEMICOLON; }
"("     { printf("LPAREN\n"); return LPAREN; }
")"     { printf("RPAREN\n"); return RPAREN; }
"+"     { printf("PLUS\n"); return PLUS; }
"-"     { printf("MINUS\n"); return MINUS; }
"*"     { printf("MULTIPLY\n"); return MULTIPLY; }
"/"     { printf("DIVIDE\n"); return DIVIDE; }
"^"     { printf("POWER\n"); return POWER; }
.       { printf("Unexpected character: %s\n", yytext); }
%%