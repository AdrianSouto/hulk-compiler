#ifndef ASTVISITOR_HPP
#define ASTVISITOR_HPP

#include "ASTNode.hpp"
#include <iostream>

class Visitor {
public:
    virtual ~Visitor() = default;
    virtual void visit(BinaryOperation& node) = 0;
    virtual void visit(NumberLiteral& node) = 0;
    virtual void visit(PrintStatement& node) = 0;
    virtual void visit(ExpressionStatement& node) = 0;
};

class PrintVisitor : public Visitor {
public:
    void visit(BinaryOperation& node) override {
        std::cout << "(";
        node.left->accept(*this);
        std::cout << " " << node.op << " ";
        node.right->accept(*this);
        std::cout << ")";
    }

    void visit(NumberLiteral& node) override {
        std::cout << node.value;
    }

    void visit(PrintStatement& node) override {
        std::cout << "print ";
        node.expression->accept(*this);
        std::cout << ";";
    }

    void visit(ExpressionStatement& node) override {
        node.expression->accept(*this);
        std::cout << ";";
    }
};

#endif // ASTVISITOR_HPP