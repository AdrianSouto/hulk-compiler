// ASTNode.hpp
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void accept(Visitor& visitor) = 0;
};

class Expression : public ASTNode {
    // Base para todas las expresiones
};

class Statement : public ASTNode {
    // Base para todas las sentencias
};

class BinaryOperation : public Expression {
    Expression* left;
    Expression* right;
    std::string op;  // "+", "-", etc.
    // Constructor y métodos...
};

class NumberLiteral : public Expression {
    double value;
    // Constructor y métodos...
};

class PrintStatement : public Statement {
    Expression* expression;
    // Constructor y métodos...
};