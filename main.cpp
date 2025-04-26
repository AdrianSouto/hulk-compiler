#include "ASTNode.hpp"

#include <iostream>
#include <fstream>
#include <vector> // Agregado para manejar el vector de nodos
#include "parser.hpp"

extern FILE* yyin;
extern int yyparse();
extern std::vector<StatementNode*> root; // Cambiado a vector de StatementNode

int main(int argc, char* argv[]) {
    const char* filename = "../input.txt";

    yyin = fopen(filename, "r");
    if (!yyin) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return 1;
    }
    yyparse();

    if (!root.empty()) {
        for (const auto& statement : root) {
            if (statement) {
                statement->print();
                statement->execute(); // Ejecutar cada nodo
            }
        }
    } else {
        std::cout << "root está vacío." << std::endl;
    }

    fclose(yyin);
    return 0;
}
