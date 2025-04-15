#ifndef ASTNODE_HPP
#define ASTNODE_HPP

#include <string>
#include <memory>

class Visitor;

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void accept(Visitor& visitor) = 0;
    static ASTNode* root; // Nodo raíz del AST
};

class Expression : public ASTNode {
};

class Statement : public ASTNode {
};

class BinaryOperation : public Expression {
public:
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
    std::string op;

    BinaryOperation(Expression* left, const std::string& op, Expression* right);
    void accept(Visitor& visitor) override;
};

class NumberLiteral : public Expression {
public:
    double value;

    explicit NumberLiteral(double value);
    void accept(Visitor& visitor) override;
};

class PrintStatement : public Statement {
public:
    std::unique_ptr<Expression> expression;

    explicit PrintStatement(Expression* expr);
    void accept(Visitor& visitor) override;
};

class ExpressionStatement : public Statement {
public:
    std::unique_ptr<Expression> expression;

    explicit ExpressionStatement(Expression* expr);
    void accept(Visitor& visitor) override;
};

// Declaración de funciones de ayuda
Expression* makeNumber(double value);
Expression* makeBinaryOp(Expression* left, const std::string& op, Expression* right);

#endif // ASTNODE_HPP