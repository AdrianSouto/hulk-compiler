#include "ASTNode.hpp"
#include <iostream>

NumberNode::NumberNode(int val) : value(val) {}

int NumberNode::evaluate() const {
    return value;
}

void NumberNode::print(int indent) const {
    for (int i = 0; i < indent; ++i) {
        std::cout << "  ";
    }
    std::cout << "Number: " << evaluate() << std::endl;
}

BinaryOperatorNode::BinaryOperatorNode(ExpressionNode* left, ExpressionNode* right)
    : left(left), right(right) {}

BinaryOperatorNode::~BinaryOperatorNode() {
    delete left;
    delete right;
}

AdditionNode::AdditionNode(ExpressionNode* left, ExpressionNode* right)
    : BinaryOperatorNode(left, right) {}

int AdditionNode::evaluate() const {
    return left->evaluate() + right->evaluate();
}

char AdditionNode::getOperator() const {
    return '+';
}

SubtractionNode::SubtractionNode(ExpressionNode* left, ExpressionNode* right)
    : BinaryOperatorNode(left, right) {}

int SubtractionNode::evaluate() const {
    return left->evaluate() - right->evaluate();
}

char SubtractionNode::getOperator() const {
    return '-';
}

MultiplicationNode::MultiplicationNode(ExpressionNode* left, ExpressionNode* right)
    : BinaryOperatorNode(left, right) {}

int MultiplicationNode::evaluate() const {
    return left->evaluate() * right->evaluate();
}

char MultiplicationNode::getOperator() const {
    return '*';
}

DivisionNode::DivisionNode(ExpressionNode* left, ExpressionNode* right)
    : BinaryOperatorNode(left, right) {}

int DivisionNode::evaluate() const {
    return left->evaluate() / right->evaluate();
}

char DivisionNode::getOperator() const {
    return '/';
}

PrintStatementNode::PrintStatementNode(ExpressionNode* expr) : expression(expr) {}

void PrintStatementNode::execute() const {
    std::cout << "Print: " << expression->evaluate() << std::endl;
}

void PrintStatementNode::print(int indent) const {
    for (int i = 0; i < indent; ++i) {
        std::cout << "  ";
    }
    std::cout << "PrintStatement:" << std::endl;
    expression->print(indent + 1);
}

PrintStatementNode::~PrintStatementNode() {
    delete expression;
}

void BinaryOperatorNode::print(int indent) const {
    for (int i = 0; i < indent; ++i) {
        std::cout << "  ";
    }
    std::cout << "Operator: " << getOperator() << std::endl;

    for (int i = 0; i < indent + 1; ++i) {
        std::cout << "  ";
    }
    std::cout << "Left: ";
    if (left) {
        left->print(indent + 1);
    } else {
        std::cout << "nullptr" << std::endl;
    }

    for (int i = 0; i < indent + 1; ++i) {
        std::cout << "  ";
    }
    std::cout << "Right: ";
    if (right) {
        right->print(indent + 1);
    } else {
        std::cout << "nullptr" << std::endl;
    }
}