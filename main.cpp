#include <iostream>
#include <fstream>
#include "ASTNode.hpp"
#include "ASTVisitor.hpp"

extern FILE* yyin;
extern int yyparse();

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <archivo_entrada>" << std::endl;
        return 1;
    }

    const char* filename = argv[1];
    yyin = fopen(filename, "r");
    if (!yyin) {
        std::cerr << "Error al abrir el archivo: " << filename << std::endl;
        return 1;
    }

    // Parsear el archivo
    yyparse();

    // Procesar el AST si se construyó correctamente
    if (ASTNode::root) {
        std::cout << "=== AST Generado ===" << std::endl;
        PrintVisitor visitor;
        ASTNode::root->accept(visitor);
        std::cout << std::endl << "===================" << std::endl;
    }

    fclose(yyin);
    return 0;
}