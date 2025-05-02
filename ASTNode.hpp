#ifndef ASTNODE_HPP
#define ASTNODE_HPP

#include <iostream>
#include <map>
#include <vector>
#include <string>

class ASTNode {
public:
    virtual void print(int indent = 0) const = 0;
    virtual ~ASTNode() = default; // Destructor virtual
};

class ExpressionNode : public ASTNode {
public:
    virtual int evaluate() const = 0;
    virtual std::string evaluateString() const { return std::to_string(evaluate()); }
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

class StringLiteralNode : public ExpressionNode {
public:
    std::string value;

    StringLiteralNode(const std::string& val);

    int evaluate() const override;
    std::string evaluateString() const override;
    void print(int indent = 0) const override;
};

class VariableNode : public ExpressionNode {
public:
    std::string identifier;

    VariableNode(const std::string& id);

    int evaluate() const override;
    std::string evaluateString() const override;
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

class ConcatenationNode : public BinaryOperatorNode {
public:
    ConcatenationNode(ExpressionNode* left, ExpressionNode* right);

    int evaluate() const override;
    std::string evaluateString() const override;
    char getOperator() const override;
};

class FuncCallNode : public ExpressionNode {
public:
    std::string identifier;
    std::vector<ExpressionNode*> args;

    FuncCallNode(const std::string& id, const std::vector<ExpressionNode*>& arguments);

    int evaluate() const override;
    void print(int indent = 0) const override;
    ~FuncCallNode();
};

class PrintStatementNode : public StatementNode {
public:
    ExpressionNode* expression;

    PrintStatementNode(ExpressionNode* expr);

    void execute() const override;
    void print(int indent = 0) const override;

    ~PrintStatementNode();
};

class LetVarNode : public StatementNode {
public:
    std::string identifier;
    ExpressionNode* expr;
    StatementNode* body;

    LetVarNode(const std::string& id, ExpressionNode* expr, StatementNode* body);

    void execute() const override;
    void print(int indent = 0) const override;
    ~LetVarNode();
};

class DefFuncNode : public StatementNode {
public:
    std::string identifier;
    std::vector<std::string> arguments;
    ExpressionNode* expr;

    DefFuncNode(const std::string& id, const std::vector<std::string>& args, ExpressionNode* expr);

    void execute() const override;
    void print(int indent = 0) const override;
    ~DefFuncNode();
};

// Global symbol tables for variables and functions
extern std::map<std::string, std::string> variables;
extern std::map<std::string, std::pair<std::vector<std::string>, ExpressionNode*>> functions;

class Program {
public:
    std::vector<StatementNode*> Statements;

    void execute() const;
    ~Program();
};

#endif
