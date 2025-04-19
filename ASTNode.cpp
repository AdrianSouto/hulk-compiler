#include <string.h>
#include <iostream>

class NumberNode {
public:
    int value;

    NumberNode(int val) : value(val) {}

    int getValue() const {
        return value;
    }

    void print() const {
        std::cout << "Number: " << getValue() << std::endl;
    }
};

class BinaryOperatorNode {
public:
    char op;
    NumberNode* left;
    NumberNode* right;

    BinaryOperatorNode(char op, NumberNode* left, NumberNode* right) : op(op), left(left), right(right) {}

    int evaluate() const {
        switch (op) {
            case '+': return left->getValue() + right->getValue();
            case '-': return left->getValue() - right->getValue();
            case '*': return left->getValue() * right->getValue();
            case '/': return left->getValue() / right->getValue();
            default: return 0;
        }
    }

    void print(int indent = 0) const {
        for (int i = 0; i < indent; ++i) {
            std::cout << "  ";
        }
        std::cout << "Result:" << evaluate() << std::endl;
        std::cout << "Operator: " << op << std::endl;
        for (int i = 0; i < indent + 1; ++i) {
            std::cout << "  ";
        }
        if (left) {

            std::cout << "Left: ";
            left->print();
        } else {
            std::cout << "Left: nullptr" << std::endl;
        }
        for (int i = 0; i < indent + 1; ++i) {
            std::cout << "  ";
        }
        if (right) {
            std::cout << "Right: ";
            right->print();
        } else {
            std::cout << "Right: nullptr" << std::endl;
        }
    }
};