%{
#include <iostream>
#include <string>
#include <cmath>
#include <memory>
#include "ASTNode.hpp"

using namespace std;

extern int yylex();
void yyerror(const char* s);
extern FILE* yyin;

Expression* makeNumber(double value);
Expression* makeBinaryOp(Expression* left, const string& op, Expression* right);

%}

%union {
    Expression* expr;
    Statement* stmt;
    double num_val;
}

%token <num_val> NUMBER
%token PRINT
%token EOL

%token LPAREN RPAREN
%token PLUS MINUS MULTIPLY DIVIDE POWER SEMICOLON

%type <expr> expression
%type <stmt> statement

%left PLUS MINUS
%left MULTIPLY DIVIDE
%right POWER
%left LPAREN RPAREN

%%

program:
    statement_list { ASTNode::root = $1; }
    ;

statement_list:
    statement_list statement EOL { $$ = $2; }
    | statement_list statement { $$ = $2; }
    | statement EOL { $$ = $1; }
    | statement { $$ = $1; }
    | EOL { $$ = nullptr; }
    ;

statement:
    PRINT expression SEMICOLON { $$ = new PrintStatement($2); }
    | expression SEMICOLON { $$ = new ExpressionStatement($1); }  // Corregido: $1 en lugar de $2
    ;

expression:
    NUMBER { $$ = makeNumber($1); }
    | LPAREN expression RPAREN { $$ = $2; }
    | expression PLUS expression { $$ = makeBinaryOp($1, "+", $3); }
    | expression MINUS expression { $$ = makeBinaryOp($1, "-", $3); }
    | expression MULTIPLY expression { $$ = makeBinaryOp($1, "*", $3); }
    | expression DIVIDE expression { $$ = makeBinaryOp($1, "/", $3); }
    | expression POWER expression { $$ = makeBinaryOp($1, "^", $3); }
    ;

%%

void yyerror(const char* s) {
    cerr << "Error de sintaxis: " << s << endl;
}