#include <string.h>
#include <iostream>

class ASTNode {
public:
    virtual void print(int indent = 0) const = 0;
    virtual ~ASTNode() = default; // Destructor virtual
};

class ExpressionNode : public ASTNode {
public:
    virtual int evaluate() const = 0;
    ~ExpressionNode() override = default;
};
class StatementNode : public ASTNode {
public:
    virtual void execute() const = 0;
    ~StatementNode() override = default;
};

class NumberNode : public ExpressionNode {
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

class BinaryOperatorNode : public ExpressionNode {
public:
    ExpressionNode* left;
    ExpressionNode* right;

    BinaryOperatorNode(ExpressionNode* left, ExpressionNode* right)
        : left(left), right(right) {}

    virtual char getOperator() const = 0;

    void print(int indent = 0) const override {
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

    ~BinaryOperatorNode() {
        delete left;
        delete right;
    }
};

class AdditionNode : public BinaryOperatorNode {
public:
    AdditionNode(ExpressionNode* left, ExpressionNode* right)
        : BinaryOperatorNode(left, right) {}

    int evaluate() const override {
        return left->evaluate() + right->evaluate();
    }

    char getOperator() const override {
        return '+';
    }
};

class SubtractionNode : public BinaryOperatorNode {
public:
    SubtractionNode(ExpressionNode* left, ExpressionNode* right)
        : BinaryOperatorNode(left, right) {}

    int evaluate() const override {
        return left->evaluate() - right->evaluate();
    }

    char getOperator() const override {
        return '-';
    }
};

class MultiplicationNode : public BinaryOperatorNode {
public:
    MultiplicationNode(ExpressionNode* left, ExpressionNode* right)
        : BinaryOperatorNode(left, right) {}

    int evaluate() const override {
        return left->evaluate() * right->evaluate();
    }

    char getOperator() const override {
        return '*';
    }
};

class DivisionNode : public BinaryOperatorNode {
public:
    DivisionNode(ExpressionNode* left, ExpressionNode* right)
        : BinaryOperatorNode(left, right) {}

    int evaluate() const override {
        return left->evaluate() / right->evaluate();
    }

    char getOperator() const override {
        return '/';
    }
};

class PrintStatementNode : public StatementNode {
public:
    ExpressionNode* expression;

    PrintStatementNode(ExpressionNode* expr) : expression(expr) {}

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