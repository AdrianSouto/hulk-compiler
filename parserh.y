%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void yyerror(const char *s);
int yylex(void);

%}

%union {
    double fval;
    char *sval;
}

%token <fval> NUMBER
%token <sval> STRING IDENTIFIER

%token PRINT FUNCTION LET IN IF ELSE ELIF TRUE FALSE WHILE FOR RANGE ASSIGN
%token SQRT SIN COS EXP LOG RAND PI E
%token ARROW LBRACE RBRACE LPAREN RPAREN
%token PLUS MINUS MULTIPLY DIVIDE POWER CONCAT SEMICOLON COMMA
%token EQ NEQ LEQ GEQ LT GT AND OR NOT MOD ERROR EOL

%type <fval> expression
%type <sval> statement_list statement

%left PLUS MINUS
%left MULTIPLY DIVIDE
%right POWER
%left LPAREN RPAREN

%%

program:
    statement_list
    ;

statement_list:
    statement_list statement EOL
    | statement EOL
    ;

statement:
    PRINT expression SEMICOLON { printf("Resultado: %f\n", $2); }
    | expression SEMICOLON { printf("Resultado: %f\n", $1); }
    ;

expression:
    NUMBER { $$ = $1; }
    | LPAREN expression RPAREN { $$ = $2; }
    | expression PLUS expression { $$ = $1 + $3; }
    | expression MINUS expression { $$ = $1 - $3; }
    | expression MULTIPLY expression { $$ = $1 * $3; }
    | expression DIVIDE expression { $$ = $1 / $3; }
    | expression POWER expression { $$ = pow($1, $3); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main(void) {
    return yyparse();
}