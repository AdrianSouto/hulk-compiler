%{
#include <iostream>
#include <string>
using namespace std;

// Declaración de yylex para el compilador
int yylex();
void yyerror(const char* s);
int count = 1;
%}

%token NUMBER
%token PRINT
%token END_LINE

%%
input:
    | input line
    | input err_semicolon
    ;

line:
    '\n' { count++; }
    | expression END_LINE  { cout << "Result: " << $1 << endl; }
    | PRINT expression END_LINE  { cout << "Print: " << $2 << endl; }
    ;

err_semicolon:
    | expression { cout << "Error: Falta el punto y coma en la linea " << count << endl; }
    | PRINT expression { cout << "Error: Falta el punto y coma en la linea " << count << endl; }
    ;

expression:
    NUMBER  { $$ = $1; }
    ;

%%
void yyerror(const char* s) {
    cerr << "Error: " << s << endl;
}