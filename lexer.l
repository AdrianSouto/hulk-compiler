%{
#include "parser.hpp"
#include <cstdio>

int yylex();
%}

%option noyywrap

%%
[0-9]+  { yylval = atoi(yytext); return NUMBER; }
print   { return PRINT; }
\n      { return '\n'; }
[ \t]   { /* ignore whitespace */ }
;       { return END_LINE; }
\+      { return '+'; }
\-      { return '-'; }
\*      { return '*'; }
\/      { return '/'; }
\(      { return '('; }
\)      { return ')'; }
.       { printf("Unexpected character: %s\n", yytext); }
%%