#ifndef LLVM_CODE_GEN_VISITOR_HPP
#define LLVM_CODE_GEN_VISITOR_HPP

#include "Visitor.hpp"
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Value.h>
#include <map>
#include <string>
#include <memory>

class LLVMCodeGenVisitor : public Visitor {
private:
    std::unique_ptr<llvm::LLVMContext> context;
    std::unique_ptr<llvm::Module> module;
    std::unique_ptr<llvm::IRBuilder<>> builder;
    std::map<std::string, llvm::Value*> namedValues;
    llvm::Value* lastValue;

public:
    LLVMCodeGenVisitor(const std::string& moduleName);
    ~LLVMCodeGenVisitor() override = default;
    
    // Get the generated module
    llvm::Module* getModule() { return module.get(); }
    
    // Get the result of the last expression
    llvm::Value* getLastValue() { return lastValue; }
    
    // Base nodes
    void visit(ASTNode* node) override;
    void visit(ExpressionNode* node) override;
    void visit(StatementNode* node) override;
    
    // Expression nodes
    void visit(NumberNode* node) override;
    void visit(StringLiteralNode* node) override;
    void visit(VariableNode* node) override;
    void visit(BinaryOperatorNode* node) override;
    void visit(AdditionNode* node) override;
    void visit(SubtractionNode* node) override;
    void visit(MultiplicationNode* node) override;
    void visit(DivisionNode* node) override;
    void visit(ConcatenationNode* node) override;
    void visit(FuncCallNode* node) override;
    
    // Statement nodes
    void visit(PrintStatementNode* node) override;
    void visit(LetVarNode* node) override;
    void visit(DefFuncNode* node) override;
    
    // Program
    void visit(Program* program) override;
    
    // Helper method to dump the generated IR
    void dumpIR();
};

#endif // LLVM_CODE_GEN_VISITOR_HPP
