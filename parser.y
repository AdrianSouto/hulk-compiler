%{
#include "ASTNode.hpp"
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

int yylex();
void yyerror(const char* s);
int count = 1;
Program program; // Cambiado a Program
%}

%token<number> NUMBER
%token PRINT
%token EOL

%token LPAREN RPAREN
%token PLUS MINUS MULTIPLY DIVIDE POWER SEMICOLON

%union {
    int number;
    ExpressionNode* expression;
    StatementNode* statement;
}

%left PLUS MINUS
%left MULTIPLY DIVIDE
%right POWER
%left LPAREN RPAREN
%type<expression> expression
%type<statement> statement

%%

program:
    statement_list
    ;

statement_list:
    statement_list statement EOL { program.Statements.push_back($2); }
    | statement_list statement { program.Statements.push_back($2); }
    | statement EOL { program.Statements.push_back($1); }
    | statement { program.Statements.push_back($1); }
    | EOL { count++; }
    ;

statement:
    PRINT expression SEMICOLON { $$ = new PrintStatementNode($2); }
    ;

expression:
    NUMBER { $$ = new NumberNode($1); }
    | LPAREN expression RPAREN { $$ = $2; }
    | expression PLUS expression { $$ = new AdditionNode($1, $3); }
    | expression MINUS expression { $$ = new SubtractionNode($1, $3); }
    | expression MULTIPLY expression { $$ = new MultiplicationNode($1, $3); }
    | expression DIVIDE expression { $$ = new DivisionNode($1, $3); }
    ;

%%
void yyerror(const char* s) {
    cerr << "Error: " << s << endl;
}
