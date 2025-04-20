#include <string.h>
#include <iostream>

class ASTNode {
public:
    virtual void print(int indent = 0) const = 0;
    virtual ~ASTNode() = default; // Destructor virtual
};

class ExpresionNode : public ASTNode {
public:
    virtual int evaluate() const = 0;
    ~ExpresionNode() override = default;
};
class StatementNode : public ASTNode {
public:
    virtual void execute() const = 0;
    ~StatementNode() override = default;
};

class NumberNode : public ExpresionNode {
public:
    int value;

    NumberNode(int val) : value(val) {}

    int evaluate() const override {
        return value;
    }

    void print(int indent = 0) const override {
        for (int i = 0; i < indent; ++i) {
            std::cout << "  ";
        }
        std::cout << "Number: " << evaluate() << std::endl;
    }
};

class BinaryOperatorNode : public ExpresionNode {
public:
    char op;
    ExpresionNode* left;
    ExpresionNode* right;

    BinaryOperatorNode(char op, ExpresionNode* left, ExpresionNode* right) : op(op), left(left), right(right) {}

    int evaluate() const override {
        switch (op) {
            case '+': return left->evaluate() + right->evaluate();
            case '-': return left->evaluate() - right->evaluate();
            case '*': return left->evaluate() * right->evaluate();
            case '/': return left->evaluate() / right->evaluate();
            default: return 0;
        }
    }

    void print(int indent = 0) const override {
        for (int i = 0; i < indent; ++i) {
            std::cout << "  ";
        }
        std::cout << "Operator: " << op << std::endl;

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

    ~BinaryOperatorNode() {
        delete left;
        delete right;
    }
};
class PrintStatementNode : public StatementNode {
public:
    ExpresionNode* expression;

    PrintStatementNode(ExpresionNode* expr) : expression(expr) {}

    void execute() const override {
        std::cout << "Print: " << expression->evaluate() << std::endl;
    }

    void print(int indent = 0) const override {
        for (int i = 0; i < indent; ++i) {
            std::cout << "  ";
        }
        std::cout << "PrintStatement:" << std::endl;
        expression->print(indent + 1);
    }

    ~PrintStatementNode() {
        delete expression;
    }
};