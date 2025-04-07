#include <iostream>
#include <fstream>
#include "parser.hpp"

extern FILE* yyin;
extern int yyparse();

int main(int argc, char* argv[]) {
    const char* filename = "../input.txt";

    yyin = fopen(filename, "r");
    if (!yyin) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return 1;
    }
    yyparse();

    fclose(yyin);
    return 0;
}