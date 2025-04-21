%{
#include <iostream>
#include <string>
#include <cmath>
#include "../ASTNode.cpp"

using namespace std;

// Declaración de yylex para el compilador
int yylex();
void yyerror(const char* s);
int count = 1;
extern BinaryOperatorNode* currentNode = nullptr;
%}

%token NUMBER
%token PRINT
%token EOL

%token LPAREN RPAREN
%token PLUS MINUS MULTIPLY DIVIDE POWER SEMICOLON

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
    statement_list statement
    | statement EOL
    | statement
    | EOL { count++; }
    ;

statement:
    PRINT expression SEMICOLON { { cout << "Print: " << $2 << endl; } }
    | expression SEMICOLON { cout << "Resultado: " << $1 << endl;  }
    ;

expression:
    NUMBER { $$ = $1; }
    | LPAREN expression RPAREN { $$ = $2; }
    | expression PLUS expression {
        $$ = $1 + $3;
        NumberNode* left = new NumberNode($1);
        NumberNode* right = new NumberNode($3);
        currentNode = new AdditionNode(left, right);
     }
    | expression MINUS expression {
        $$ = $1 - $3;
        NumberNode* left = new NumberNode($1);
        NumberNode* right = new NumberNode($3);
        currentNode = new SubtractionNode(left, right);
     }
    | expression MULTIPLY expression {
        $$ = $1 * $3;
        NumberNode* left = new NumberNode($1);
        NumberNode* right = new NumberNode($3);
        currentNode = new MultiplicationNode(left, right);
     }
    | expression DIVIDE expression {
        $$ = $1 / $3;
        NumberNode* left = new NumberNode($1);
        NumberNode* right = new NumberNode($3);
        currentNode = new DivisionNode(left, right);
     }

%%
void yyerror(const char* s) {
    cerr << "Error: " << s << endl;
}