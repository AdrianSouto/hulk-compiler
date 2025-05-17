#ifndef VISITOR_HPP
#define VISITOR_HPP

// Forward declarations for all node types
class ASTNode;
class ExpressionNode;
class StatementNode;
class NumberNode;
class StringLiteralNode;
class VariableNode;
class BinaryOperatorNode;
class AdditionNode;
class SubtractionNode;
class MultiplicationNode;
class DivisionNode;
class ConcatenationNode;
class FuncCallNode;
class PrintStatementNode;
class LetVarNode;
class DefFuncNode;
class Program;

// The base visitor interface
class Visitor {
public:
    virtual ~Visitor() = default;
    
    // Base nodes
    virtual void visit(ASTNode* node) = 0;
    virtual void visit(ExpressionNode* node) = 0;
    virtual void visit(StatementNode* node) = 0;
    
    // Expression nodes
    virtual void visit(NumberNode* node) = 0;
    virtual void visit(StringLiteralNode* node) = 0;
    virtual void visit(VariableNode* node) = 0;
    virtual void visit(BinaryOperatorNode* node) = 0;
    virtual void visit(AdditionNode* node) = 0;
    virtual void visit(SubtractionNode* node) = 0;
    virtual void visit(MultiplicationNode* node) = 0;
    virtual void visit(DivisionNode* node) = 0;
    virtual void visit(ConcatenationNode* node) = 0;
    virtual void visit(FuncCallNode* node) = 0;
    
    // Statement nodes
    virtual void visit(PrintStatementNode* node) = 0;
    virtual void visit(LetVarNode* node) = 0;
    virtual void visit(DefFuncNode* node) = 0;
    
    // Program
    virtual void visit(Program* program) = 0;
};

#endif // VISITOR_HPP
