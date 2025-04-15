%{
#include "y.tab.h"
#include <stdlib.h>  // Para atof()
#include <string.h>  // Para strdup()
%}

%%
[0-9]+(\.[0-9]+)?  { yylval.fval = atof(yytext); printf("NUMBER: %f\n", yylval.fval); return NUMBER; }
\"([^\\\"]|\\[\"nt])*\"  { yylval.sval = strdup(yytext); printf("STRING: %s\n", yylval.sval); return STRING; }
"print"            { printf("PRINT\n"); return PRINT; }
"function"         { printf("FUNCTION\n"); return FUNCTION; }
"let"              { printf("LET\n"); return LET; }
"in"               { printf("IN\n"); return IN; }
"if"               { printf("IF\n"); return IF; }
"else"             { printf("ELSE\n"); return ELSE; }
"elif"             { printf("ELIF\n"); return ELIF; }
"true"             { printf("TRUE\n"); return TRUE; }
"false"            { printf("FALSE\n"); return FALSE; }
"while"            { printf("WHILE\n"); return WHILE; }
"for"              { printf("FOR\n"); return FOR; }
"range"            { printf("RANGE\n"); return RANGE; }
":="               { printf("ASSIGN\n"); return ASSIGN; }
"sqrt"             { printf("SQRT\n"); return SQRT; }
"sin"              { printf("SIN\n"); return SIN; }
"cos"              { printf("COS\n"); return COS; }
"exp"              { printf("EXP\n"); return EXP; }
"log"              { printf("LOG\n"); return LOG; }
"rand"             { printf("RAND\n"); return RAND; }
"PI"               { yylval.fval = 3.141592653589793; printf("PI: %f\n", yylval.fval); return PI; }
"E"                { yylval.fval = 2.718281828459045; printf("E: %f\n", yylval.fval); return E; }
"=>"               { printf("ARROW\n"); return ARROW; }
"{"                { printf("LBRACE\n"); return LBRACE; }
"}"                { printf("RBRACE\n"); return RBRACE; }
"("                { printf("LPAREN\n"); return LPAREN; }
")"                { printf("RPAREN\n"); return RPAREN; }
"+"                { printf("PLUS\n"); return PLUS; }
"-"                { printf("MINUS\n"); return MINUS; }
"*"                { printf("MULTIPLY\n"); return MULTIPLY; }
"/"                { printf("DIVIDE\n"); return DIVIDE; }
"^"                { printf("POWER\n"); return POWER; }
"@"                { printf("CONCAT\n"); return CONCAT; }
";"                { printf("SEMICOLON\n"); return SEMICOLON; }
","                { printf("COMMA\n"); return COMMA; }
"=="               { printf("EQ\n"); return EQ; }
"!="               { printf("NEQ\n"); return NEQ; }
"<="               { printf("LEQ\n"); return LEQ; }
">="               { printf("GEQ\n"); return GEQ; }
"<"                { printf("LT\n"); return LT; }
">"                { printf("GT\n"); return GT; }
"&"                { printf("AND\n"); return AND; }
"|"                { printf("OR\n"); return OR; }
"!"                { printf("NOT\n"); return NOT; }
"%"                { printf("MOD\n"); return MOD; }
[ \t]              ;  // Ignora espacios y tabs
\n                 { printf("EOL\n"); return EOL; }
[a-zA-Z][a-zA-Z0-9_]*  { yylval.sval = strdup(yytext); printf("IDENTIFIER: %s\n", yylval.sval); return IDENTIFIER; }
.                  { printf("ERROR: %s\n", yytext); return ERROR; }
%%

int yywrap() {
    return 1;  // Función requerida en versiones antiguas de Flex
}