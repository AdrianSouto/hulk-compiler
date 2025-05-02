#include "ASTNode.hpp"
#include <iostream>
#include <fstream>
#include "parser.hpp"

extern FILE* yyin;
extern int yyparse();
extern Program program; // Cambiado a Program

int main(int argc, char* argv[]) {
    const char* filename = "../input.txt";

    yyin = fopen(filename, "r");
    if (!yyin) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return 1;
    }
    yyparse();

    if (!program.Statements.empty()) {
        program.execute(); // Ejecutar el programa
    } else {
        std::cout << "No hay declaraciones en el programa." << std::endl;
    }

    fclose(yyin);
    return 0;
}
