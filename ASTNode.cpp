#include "ASTNode.hpp"
#include "ASTVisitor.hpp"

ASTNode* ASTNode::root = nullptr;

BinaryOperation::BinaryOperation(Expression* left, const std::string& op, Expression* right)
    : left(left), right(right), op(op) {}

void BinaryOperation::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NumberLiteral::NumberLiteral(double value) : value(value) {}

void NumberLiteral::accept(Visitor& visitor) {
    visitor.visit(*this);
}

PrintStatement::PrintStatement(Expression* expr) : expression(expr) {}

void PrintStatement::accept(Visitor& visitor) {
    visitor.visit(*this);
}

ExpressionStatement::ExpressionStatement(Expression* expr) : expression(expr) {}

void ExpressionStatement::accept(Visitor& visitor) {
    visitor.visit(*this);
}

// Implementación de funciones de ayuda
Expression* makeNumber(double value) {
    return new NumberLiteral(value);
}

Expression* makeBinaryOp(Expression* left, const std::string& op, Expression* right) {
    return new BinaryOperation(left, op, right);
}