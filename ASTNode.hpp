#ifndef ASTNODE_HPP
#define ASTNODE_HPP

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <sstream>

// Forward declarations
class IContext;

class ASTNode {
protected:
    std::string errorMessage;
public:
    virtual void print(int indent = 0) const = 0;
    virtual bool validate(IContext* context) = 0;
    virtual ~ASTNode() = default; // Destructor virtual
    
    const std::string& getErrorMessage() const { return errorMessage; }
    void setErrorMessage(const std::string& message) { errorMessage = message; }
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

// IContext interface definition
class IContext {
public:
    virtual bool IsDefined(const std::string& variable) = 0;
    virtual bool IsDefined(const std::string& function, int args) = 0;
    virtual bool Define(const std::string& variable) = 0;
    virtual bool Define(const std::string& function, const std::vector<std::string>& args) = 0;
    virtual IContext* CreateChildContext() = 0;
    virtual ~IContext() = default;
};

// Context implementation
class Context : public IContext {
private:
    IContext* parent;
    std::unordered_set<std::string> variables;
    std::unordered_map<std::string, std::vector<std::string>> functions;

public:
    Context() : parent(nullptr) {}
    Context(IContext* parent) : parent(parent) {}

    bool IsDefined(const std::string& variable) override {
        return variables.find(variable) != variables.end() ||
               (parent != nullptr && parent->IsDefined(variable));
    }

    bool IsDefined(const std::string& function, int args) override {
        auto it = functions.find(function);
        if (it != functions.end() && it->second.size() == args) {
            return true;
        }
        return parent != nullptr && parent->IsDefined(function, args);
    }

    bool Define(const std::string& variable) override {
        if (variables.find(variable) != variables.end()) {
            return false;
        }
        variables.insert(variable);
        return true;
    }

    bool Define(const std::string& function, const std::vector<std::string>& args) override {
        auto it = functions.find(function);
        if (it != functions.end() && it->second.size() == args.size()) {
            return false;
        }
        functions[function] = args;
        return true;
    }

    IContext* CreateChildContext() override {
        return new Context(this);
    }

    ~Context() override = default;
};

class NumberNode : public ExpressionNode {
public:
    int value;

    NumberNode(int val);

    int evaluate() const override;
    void print(int indent = 0) const override;
    bool validate(IContext* context) override { return true; }
};

class StringLiteralNode : public ExpressionNode {
public:
    std::string value;

    StringLiteralNode(const std::string& val);

    int evaluate() const override;
    std::string evaluateString() const override;
    void print(int indent = 0) const override;
    bool validate(IContext* context) override { return true; }
};

class VariableNode : public ExpressionNode {
public:
    std::string identifier;

    VariableNode(const std::string& id);

    int evaluate() const override;
    std::string evaluateString() const override;
    void print(int indent = 0) const override;
    bool validate(IContext* context) override;
};

class BinaryOperatorNode : public ExpressionNode {
public:
    ExpressionNode* left;
    ExpressionNode* right;

    BinaryOperatorNode(ExpressionNode* left, ExpressionNode* right);

    virtual char getOperator() const = 0;

    void print(int indent = 0) const override;
    bool validate(IContext* context) override;

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
    bool validate(IContext* context) override;
    ~FuncCallNode();
};

class PrintStatementNode : public StatementNode {
public:
    ExpressionNode* expression;

    PrintStatementNode(ExpressionNode* expr);

    void execute() const override;
    void print(int indent = 0) const override;
    bool validate(IContext* context) override;

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
    bool validate(IContext* context) override;
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
    bool validate(IContext* context) override;
    ~DefFuncNode();
};

// Global symbol tables for variables and functions
extern std::map<std::string, std::string> variables;
extern std::map<std::string, std::pair<std::vector<std::string>, ExpressionNode*>> functions;

class Program {
public:
    std::vector<StatementNode*> Statements;
    std::string errorMessage;

    void execute() const;
    bool validate();
    const std::string& getErrorMessage() const { return errorMessage; }
    ~Program();
};

#endif
