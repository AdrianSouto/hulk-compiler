%{
#include <iostream>
#include <string>
using namespace std;

// Declaración de yylex para el compilador
int yylex();
void yyerror(const char* s);
%}

%token NUMBER
%token PRINT
%token END_LINE

%%
input:
    | input line
    ;

line:
    '\n'
    | expression END_LINE '\n'  { cout << "Result: " << $1 << endl; }
    | expression END_LINE  { cout << "Result: " << $1 << endl; }
    | PRINT expression END_LINE '\n'  { cout << "Print: " << $2 << endl; }
    | PRINT expression END_LINE  { cout << "Print: " << $2 << endl; }
    ;

err_semicolon:
    | expression '\n' { cout << "Error: Falta el punto y coma" << endl; }
    | PRINT expression '\n' { cout << "Error: Falta el punto y coma" << endl; }
    ;

expression:
    NUMBER  { $$ = $1; }
    ;

%%
void yyerror(const char* s) {
    cerr << "Error: " << s << endl;
}