#ifndef ASTNODE_HPP
#define ASTNODE_HPP

#include <iostream>
#include <vector>

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

    NumberNode(int val);

    int evaluate() const override;
    void print(int indent = 0) const override;
};

class BinaryOperatorNode : public ExpressionNode {
public:
    ExpressionNode* left;
    ExpressionNode* right;

    BinaryOperatorNode(ExpressionNode* left, ExpressionNode* right);

    virtual char getOperator() const = 0;

    void print(int indent = 0) const override;

    ~BinaryOperatorNode();
};

class AdditionNode : public BinaryOperatorNode {
public:
    AdditionNode(ExpressionNode* left, ExpressionNode* right);

    int evaluate() const override;
    char getOperator() const override;
};

class SubtractionNode : public BinaryOperatorNode {
public:
    SubtractionNode(ExpressionNode* left, ExpressionNode* right);

    int evaluate() const override;
    char getOperator() const override;
};

class MultiplicationNode : public BinaryOperatorNode {
public:
    MultiplicationNode(ExpressionNode* left, ExpressionNode* right);

    int evaluate() const override;
    char getOperator() const override;
};

class DivisionNode : public BinaryOperatorNode {
public:
    DivisionNode(ExpressionNode* left, ExpressionNode* right);

    int evaluate() const override;
    char getOperator() const override;
};

class PrintStatementNode : public StatementNode {
public:
    ExpressionNode* expression;

    PrintStatementNode(ExpressionNode* expr);

    void execute() const override;
    void print(int indent = 0) const override;

    ~PrintStatementNode();
};

class Program {
public:
    std::vector<StatementNode*> Statements;

    void execute() const {
        for (const auto& statement : Statements) {
            if (statement) {
                statement->print();
                statement->execute();
            }
        }
    }

    ~Program() {
        for (auto statement : Statements) {
            delete statement;
        }
    }
};

#endif