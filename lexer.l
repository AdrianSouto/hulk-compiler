%{
#include "ASTNode.hpp" // Mover este include antes de parser.hpp
#include "parser.hpp"
#include <cstdio>
#include <cstring>

int yylex();
%}

%option noyywrap

%%
[0-9]+  { yylval.number = atoi(yytext); printf("NUMBER\n"); return NUMBER; }
\"[^\"]*\" { 
    yylval.string = strdup(yytext + 1); // Skip the opening quote
    yylval.string[strlen(yylval.string) - 1] = '\0'; // Remove the closing quote
    printf("STRING: %s\n", yylval.string);
    return STRING; 
}
print   { printf("PRINT\n"); return PRINT; }
let     { printf("LET\n"); return LET; }
in      { printf("IN\n"); return IN; }
function { printf("FUNCTION\n"); return FUNCTION; }
=>      { printf("ARROW\n"); return ARROW; }
[a-zA-Z][a-zA-Z0-9_]* { 
    yylval.string = strdup(yytext);
    printf("IDENTIFIER: %s\n", yylval.string);
    return IDENTIFIER; 
}
\n      { return EOL; }
[ \t]   { /* ignore whitespace */ }
;       { return SEMICOLON; }
=       { return EQUALS; }
,       { return COMMA; }
"("     { printf("LPAREN\n"); return LPAREN; }
")"     { printf("RPAREN\n"); return RPAREN; }
"+"     { printf("PLUS\n"); return PLUS; }
"-"     { printf("MINUS\n"); return MINUS; }
"*"     { printf("MULTIPLY\n"); return MULTIPLY; }
"/"     { printf("DIVIDE\n"); return DIVIDE; }
"^"     { printf("POWER\n"); return POWER; }
"@"     { printf("CONCAT\n"); return CONCAT; }
.       { printf("Unexpected character: %s\n", yytext); }
%%
