#include "ASTNode.hpp"
#include "LLVMCodeGenVisitor.hpp"
#include <iostream>
#include <fstream>
#include "parser.hpp"

extern FILE* yyin;
extern int yyparse();
extern Program program;

int main(int argc, char* argv[]) {
    const char* filename = "../input.txt";

    yyin = fopen(filename, "r");
    if (!yyin) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return 1;
    }
    yyparse();

    if (!program.Statements.empty()) {
        // Validate the program before executing
        if (program.validate()) {
            std::cout << "Program successfully validated!" << std::endl;
            
            // Generate LLVM IR code
            LLVMCodeGenVisitor codeGen("hulk_module");
            program.accept(&codeGen);
            
            // Print the generated IR code
            std::cout << "\nGenerated LLVM IR:\n";
            codeGen.dumpIR();
            
            try {
                program.execute(); // Execute the program
            } catch (const std::exception& e) {
                std::cerr << "Runtime error: " << e.what() << std::endl;
                return 1;
            }
        } else {
            std::cerr << "Program validation failed: " << program.getErrorMessage() << std::endl;
            return 1;
        }
    } else {
        std::cout << "No hay declaraciones en el programa." << std::endl;
    }

    fclose(yyin);
    return 0;
}
