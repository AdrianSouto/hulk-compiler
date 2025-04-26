%{
#include "ASTNode.hpp" // Mover este include antes de parser.hpp
#include "parser.hpp"
#include <cstdio>
#include <cstring>

int yylex();
%}

%option noyywrap

%%
[0-9]+  { yylval.number = atoi(yytext);printf("NUMBER\n"); return NUMBER; }
print   {printf("PRINT\n"); return PRINT; }
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

