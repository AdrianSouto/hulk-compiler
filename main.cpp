#include <iostream>
#include <fstream>
#include "parser.hpp"
#include "../ASTNode.cpp"

extern FILE* yyin;
extern int yyparse();
extern BinaryOperatorNode* currentNode;

int main(int argc, char* argv[]) {
    const char* filename = "../input.txt";

    yyin = fopen(filename, "r");
    if (!yyin) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return 1;
    }
    yyparse();

    if (currentNode) {
        currentNode->print();
    } else {
        std::cout << "currentNode es nulo." << std::endl;
    }

    fclose(yyin);
    return 0;
}