#include "LLVMCodeGenVisitor.hpp"
#include "ASTNode.hpp"
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>
#include <iostream>

LLVMCodeGenVisitor::LLVMCodeGenVisitor(const std::string& moduleName) {
    // Initialize LLVM components
    context = std::make_unique<llvm::LLVMContext>();
    module = std::make_unique<llvm::Module>(moduleName, *context);
    builder = std::make_unique<llvm::IRBuilder<>>(*context);
    lastValue = nullptr;
}

void LLVMCodeGenVisitor::visit(ASTNode* node) {
    // Base implementation - do nothing
}

void LLVMCodeGenVisitor::visit(ExpressionNode* node) {
    // Base implementation - do nothing
}

void LLVMCodeGenVisitor::visit(StatementNode* node) {
    // Base implementation - do nothing
}

void LLVMCodeGenVisitor::visit(NumberNode* node) {
    // Create an integer constant
    lastValue = llvm::ConstantInt::get(*context, llvm::APInt(32, node->value, true));
}

void LLVMCodeGenVisitor::visit(StringLiteralNode* node) {
    // For now, just represent strings as their length as integers
    // In a real implementation, you'd handle actual strings
    lastValue = llvm::ConstantInt::get(*context, llvm::APInt(32, node->value.length(), true));
}

void LLVMCodeGenVisitor::visit(VariableNode* node) {
    // Look up the variable in the symbol table
    llvm::Value* value = namedValues[node->identifier];
    if (!value) {
        std::cerr << "Unknown variable name: " << node->identifier << std::endl;
        lastValue = nullptr;
        return;
    }
    
    lastValue = value;
}

void LLVMCodeGenVisitor::visit(BinaryOperatorNode* node) {
    // This will be implemented by derived classes
}

void LLVMCodeGenVisitor::visit(AdditionNode* node) {
    // Generate code for left and right operands
    node->left->accept(this);
    llvm::Value* L = lastValue;
    
    node->right->accept(this);
    llvm::Value* R = lastValue;
    
    if (!L || !R) {
        lastValue = nullptr;
        return;
    }
    
    // Create add instruction
    lastValue = builder->CreateAdd(L, R, "addtmp");
}

void LLVMCodeGenVisitor::visit(SubtractionNode* node) {
    // Generate code for left and right operands
    node->left->accept(this);
    llvm::Value* L = lastValue;
    
    node->right->accept(this);
    llvm::Value* R = lastValue;
    
    if (!L || !R) {
        lastValue = nullptr;
        return;
    }
    
    // Create sub instruction
    lastValue = builder->CreateSub(L, R, "subtmp");
}

void LLVMCodeGenVisitor::visit(MultiplicationNode* node) {
    // Generate code for left and right operands
    node->left->accept(this);
    llvm::Value* L = lastValue;
    
    node->right->accept(this);
    llvm::Value* R = lastValue;
    
    if (!L || !R) {
        lastValue = nullptr;
        return;
    }
    
    // Create mul instruction
    lastValue = builder->CreateMul(L, R, "multmp");
}

void LLVMCodeGenVisitor::visit(DivisionNode* node) {
    // Generate code for left and right operands
    node->left->accept(this);
    llvm::Value* L = lastValue;
    
    node->right->accept(this);
    llvm::Value* R = lastValue;
    
    if (!L || !R) {
        lastValue = nullptr;
        return;
    }
    
    // Create sdiv instruction (signed division)
    lastValue = builder->CreateSDiv(L, R, "divtmp");
}

void LLVMCodeGenVisitor::visit(ConcatenationNode* node) {
    // For now, just return a constant 0 as we're not handling string operations properly
    lastValue = llvm::ConstantInt::get(*context, llvm::APInt(32, 0, true));
}

void LLVMCodeGenVisitor::visit(FuncCallNode* node) {
    // Get the function from the module
    llvm::Function* callee = module->getFunction(node->identifier);
    if (!callee) {
        std::cerr << "Unknown function: " << node->identifier << std::endl;
        lastValue = nullptr;
        return;
    }
    
    // Check arguments count
    if (callee->arg_size() != node->args.size()) {
        std::cerr << "Incorrect number of arguments passed to function: " << node->identifier << std::endl;
        lastValue = nullptr;
        return;
    }
    
    // Generate code for each argument
    std::vector<llvm::Value*> argsValues;
    for (auto& arg : node->args) {
        arg->accept(this);
        if (!lastValue) {
            std::cerr << "Error in function argument generation" << std::endl;
            lastValue = nullptr;
            return;
        }
        argsValues.push_back(lastValue);
    }
    
    // Create call instruction
    lastValue = builder->CreateCall(callee, argsValues, "calltmp");
}

void LLVMCodeGenVisitor::visit(PrintStatementNode* node) {
    // Generate code for the expression to print
    node->expression->accept(this);
    
    // For now, we don't have a proper print function, so we'll just evaluate the expression
    // In a real implementation, we would call a runtime function to print the value
}

void LLVMCodeGenVisitor::visit(LetVarNode* node) {
    // Generate code for the expression
    node->expr->accept(this);
    llvm::Value* initVal = lastValue;
    
    if (!initVal) {
        std::cerr << "Error initializing variable: " << node->identifier << std::endl;
        return;
    }
    
    // Store the old value of the variable if it exists
    llvm::Value* oldValue = namedValues[node->identifier];
    
    // Add the variable to the symbol table
    namedValues[node->identifier] = initVal;
    
    // Generate code for the body
    node->body->accept(this);
    
    // Restore the old value or remove the variable from the symbol table
    if (oldValue) {
        namedValues[node->identifier] = oldValue;
    } else {
        namedValues.erase(node->identifier);
    }
}

void LLVMCodeGenVisitor::visit(DefFuncNode* node) {
    // Create function type: int(int, int, ...)
    std::vector<llvm::Type*> argTypes(node->arguments.size(), llvm::Type::getInt32Ty(*context));
    llvm::FunctionType* funcType = llvm::FunctionType::get(llvm::Type::getInt32Ty(*context), argTypes, false);
    
    // Create function
    llvm::Function* function = llvm::Function::Create(
        funcType, llvm::Function::ExternalLinkage, node->identifier, module.get());
    
    // Name the arguments
    unsigned idx = 0;
    for (auto &arg : function->args()) {
        arg.setName(node->arguments[idx++]);
    }
    
    // Create a new basic block to start insertion into
    llvm::BasicBlock* bb = llvm::BasicBlock::Create(*context, "entry", function);
    builder->SetInsertPoint(bb);
    
    // Record the function arguments in the named values map
    namedValues.clear();
    for (auto &arg : function->args()) {
        namedValues[arg.getName().str()] = &arg;
    }
    
    // Generate code for the function body
    node->expr->accept(this);
    
    if (lastValue) {
        // Create the return instruction
        builder->CreateRet(lastValue);
        
        // Validate the generated code, checking for consistency
        llvm::verifyFunction(*function);
    } else {
        // Error reading body, remove function
        function->eraseFromParent();
    }
}

void LLVMCodeGenVisitor::visit(Program* program) {
    // Visit each statement in the program
    for (auto& stmt : program->Statements) {
        stmt->accept(this);
    }
}

void LLVMCodeGenVisitor::dumpIR() {
    // Print the generated LLVM IR
    module->print(llvm::outs(), nullptr);
}
