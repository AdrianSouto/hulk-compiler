%{
#include "ASTNode.hpp"
#include <iostream>
#include <string>
#include <cmath>
#include <vector>

using namespace std;

int yylex();
void yyerror(const char* s);
int count = 1;
Program program;
%}

%token<number> NUMBER
%token<string> STRING IDENTIFIER
%token PRINT
%token EOL
%token LET IN FUNCTION ARROW
%token LPAREN RPAREN
%token PLUS MINUS MULTIPLY DIVIDE POWER CONCAT SEMICOLON COMMA EQUALS

%union {
    int number;
    char* string;
    ExpressionNode* expression;
    StatementNode* statement;
    std::vector<ExpressionNode*>* explist;
    std::vector<std::string>* idlist;
}

%left CONCAT
%left PLUS MINUS
%left MULTIPLY DIVIDE
%right POWER
%left LPAREN RPAREN
%type<expression> expression
%type<statement> statement
%type<explist> expression_list
%type<idlist> identifier_list

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
    | LET IDENTIFIER EQUALS expression IN statement { $$ = new LetVarNode($2, $4, $6); }
    | FUNCTION IDENTIFIER LPAREN identifier_list RPAREN ARROW expression SEMICOLON { 
        $$ = new DefFuncNode($2, *$4, $7); 
        delete $4;
    }
    ;

expression:
    NUMBER { $$ = new NumberNode($1); }
    | STRING { $$ = new StringLiteralNode($1); }
    | IDENTIFIER { $$ = new VariableNode($1); }
    | LPAREN expression RPAREN { $$ = $2; }
    | expression PLUS expression { $$ = new AdditionNode($1, $3); }
    | expression MINUS expression { $$ = new SubtractionNode($1, $3); }
    | expression MULTIPLY expression { $$ = new MultiplicationNode($1, $3); }
    | expression DIVIDE expression { $$ = new DivisionNode($1, $3); }
    | expression CONCAT expression { $$ = new ConcatenationNode($1, $3); }
    | IDENTIFIER LPAREN expression_list RPAREN { $$ = new FuncCallNode($1, *$3); delete $3; }
    | IDENTIFIER LPAREN RPAREN { $$ = new FuncCallNode($1, {}); }
    ;

expression_list:
    expression { $$ = new std::vector<ExpressionNode*>; $$->push_back($1); }
    | expression_list COMMA expression { $1->push_back($3); $$ = $1; }
    ;

identifier_list:
    IDENTIFIER { $$ = new std::vector<std::string>; $$->push_back($1); }
    | identifier_list COMMA IDENTIFIER { $1->push_back($3); $$ = $1; }
    |  { $$ = new std::vector<std::string>; }
    ;

%%
void yyerror(const char* s) {
    cerr << "Error: " << s << endl;
}
