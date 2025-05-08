#include "ASTNode.hpp"
#include <iostream>
#include <map>
#include <unordered_set>

// Global symbol tables
std::map<std::string, std::string> variables;
std::map<std::string, std::pair<std::vector<std::string>, ExpressionNode*>> functions;

bool VariableNode::validate(IContext* context) {
    if (!context->IsDefined(identifier)) {
        errorMessage = "Error: Variable '" + identifier + "' is not defined in the current scope";
        return false;
    }
    return true;
}

bool FuncCallNode::validate(IContext* context) {
    for (size_t i = 0; i < args.size(); i++) {
        if (!args[i]->validate(context)) {
            errorMessage = "Error in argument " + std::to_string(i+1) + " of function '" + 
                           identifier + "': " + args[i]->getErrorMessage();
            return false;
        }
    }
    
    if (!context->IsDefined(identifier, args.size())) {
        errorMessage = "Error: Function '" + identifier + "' with " + 
                       std::to_string(args.size()) + " argument(s) is not defined";
        return false;
    }
    
    return true;
}

bool PrintStatementNode::validate(IContext* context) {
    if (!expression->validate(context)) {
        errorMessage = "Error in print statement: " + expression->getErrorMessage();
        return false;
    }
    return true;
}

bool LetVarNode::validate(IContext* context) {
    if (!expr->validate(context)) {
        errorMessage = "Error in variable initialization '" + identifier + "': " + 
                       expr->getErrorMessage();
        return false;
    }
    
    if (!context->Define(identifier)) {
        errorMessage = "Error: Variable '" + identifier + "' is already defined in this scope";
        return false;
    }
    
    if (!body->validate(context)) {
        errorMessage = "Error in 'let' statement body: " + body->getErrorMessage();
        return false;
    }
    
    return true;
}

bool BinaryOperatorNode::validate(IContext* context) {
    if (!left->validate(context)) {
        errorMessage = "Error in left operand of '" + std::string(1, getOperator()) + "': " + 
                       left->getErrorMessage();
        return false;
    }
    
    if (!right->validate(context)) {
        errorMessage = "Error in right operand of '" + std::string(1, getOperator()) + "': " + 
                       right->getErrorMessage();
        return false;
    }
    
    return true;
}

bool DefFuncNode::validate(IContext* context) {
    IContext* innerContext = context->CreateChildContext();
    
    // Check for duplicate argument names
    std::unordered_set<std::string> argSet;
    for (const auto& arg : arguments) {
        if (argSet.find(arg) != argSet.end()) {
            errorMessage = "Error in function '" + identifier + "': Duplicate argument name '" + arg + "'";
            delete innerContext;
            return false;
        }
        argSet.insert(arg);
        innerContext->Define(arg);
    }
    
    if (!expr->validate(innerContext)) {
        errorMessage = "Error in function '" + identifier + "' body: " + expr->getErrorMessage();
        delete innerContext;
        return false;
    }
    
    if (!context->Define(identifier, arguments)) {
        errorMessage = "Error: Function '" + identifier + "' with " + 
                       std::to_string(arguments.size()) + " arguments is already defined";
        delete innerContext;
        return false;
    }
    
    delete innerContext;
    return true;
}

bool Program::validate() {
    Context* context = new Context();
    bool valid = true;

    for (size_t i = 0; i < Statements.size(); i++) {
        if (!Statements[i]->validate(context)) {
            errorMessage = "Error in statement " + std::to_string(i+1) + ": " + 
                          Statements[i]->getErrorMessage();
            valid = false;
            break;
        }
    }

    delete context;
    return valid;
}

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

void Program::execute() const {
    for (const auto& statement : Statements) {
        if (statement) {
            statement->print();
            statement->execute();
        }
    }
}

Program::~Program() {
    for (auto statement : Statements) {
        delete statement;
    }
}

StringLiteralNode::StringLiteralNode(const std::string& val) : value(val) {}

int StringLiteralNode::evaluate() const {
    try {
        return std::stoi(value);
    } catch(...) {
        return 0;
    }
}

std::string StringLiteralNode::evaluateString() const {
    return value;
}

void StringLiteralNode::print(int indent) const {
    for (int i = 0; i < indent; ++i) {
        std::cout << "  ";
    }
    std::cout << "String: \"" << value << "\"" << std::endl;
}

VariableNode::VariableNode(const std::string& id) : identifier(id) {}

int VariableNode::evaluate() const {
    if (variables.find(identifier) != variables.end()) {
        try {
            return std::stoi(variables[identifier]);
        } catch(...) {
            return 0;
        }
    }
    std::cerr << "Error: Variable '" << identifier << "' not defined." << std::endl;
    return 0;
}

std::string VariableNode::evaluateString() const {
    if (variables.find(identifier) != variables.end()) {
        return variables[identifier];
    }
    std::cerr << "Error: Variable '" << identifier << "' not defined." << std::endl;
    return "";
}

void VariableNode::print(int indent) const {
    for (int i = 0; i < indent; ++i) {
        std::cout << "  ";
    }
    std::cout << "Variable: " << identifier << std::endl;
}

ConcatenationNode::ConcatenationNode(ExpressionNode* left, ExpressionNode* right)
    : BinaryOperatorNode(left, right) {}

int ConcatenationNode::evaluate() const {
    return 0;
}

std::string ConcatenationNode::evaluateString() const {
    return left->evaluateString() + right->evaluateString();
}

char ConcatenationNode::getOperator() const {
    return '@';
}

FuncCallNode::FuncCallNode(const std::string& id, const std::vector<ExpressionNode*>& arguments)
    : identifier(id), args(arguments) {}

int FuncCallNode::evaluate() const {
    if (functions.find(identifier) != functions.end()) {
        auto& func = functions[identifier];
        
        std::map<std::string, std::string> origValues;
        for (size_t i = 0; i < func.first.size() && i < args.size(); ++i) {
            std::string argName = func.first[i];
            if (variables.find(argName) != variables.end()) {
                origValues[argName] = variables[argName];
            }
            variables[argName] = args[i]->evaluateString();
        }

        int result = func.second->evaluate();

        for (const auto& pair : origValues) {
            variables[pair.first] = pair.second;
        }

        return result;
    }
    std::cerr << "Error: Function '" << identifier << "' not defined." << std::endl;
    return 0;
}

void FuncCallNode::print(int indent) const {
    for (int i = 0; i < indent; ++i) {
        std::cout << "  ";
    }
    std::cout << "FunctionCall: " << identifier << "()" << std::endl;
    for (size_t i = 0; i < args.size(); ++i) {
        for (int j = 0; j < indent + 1; ++j) {
            std::cout << "  ";
        }
        std::cout << "Arg " << i << ": ";
        args[i]->print(indent + 2);
    }
}

FuncCallNode::~FuncCallNode() {
    for (auto arg : args) {
        delete arg;
    }
}

LetVarNode::LetVarNode(const std::string& id, ExpressionNode* e, StatementNode* b)
    : identifier(id), expr(e), body(b) {}

void LetVarNode::execute() const {
    std::string oldValue;
    bool hadValue = false;
    if (variables.find(identifier) != variables.end()) {
        oldValue = variables[identifier];
        hadValue = true;
    }

    variables[identifier] = expr->evaluateString();

    body->execute();

    if (hadValue) {
        variables[identifier] = oldValue;
    } else {
        variables.erase(identifier);
    }
}

void LetVarNode::print(int indent) const {
    for (int i = 0; i < indent; ++i) {
        std::cout << "  ";
    }
    std::cout << "LetVar: " << identifier << std::endl;
    
    for (int i = 0; i < indent + 1; ++i) {
        std::cout << "  ";
    }
    std::cout << "Value: ";
    expr->print(indent + 2);
    
    for (int i = 0; i < indent + 1; ++i) {
        std::cout << "  ";
    }
    std::cout << "Body: ";
    body->print(indent + 2);
}

LetVarNode::~LetVarNode() {
    delete expr;
    delete body;
}

DefFuncNode::DefFuncNode(const std::string& id, const std::vector<std::string>& args, ExpressionNode* e)
    : identifier(id), arguments(args), expr(e) {}

void DefFuncNode::execute() const {
    functions[identifier] = std::make_pair(arguments, expr);
}

void DefFuncNode::print(int indent) const {
    for (int i = 0; i < indent; ++i) {
        std::cout << "  ";
    }
    std::cout << "DefFunc: " << identifier << "(";
    for (size_t i = 0; i < arguments.size(); ++i) {
        std::cout << arguments[i];
        if (i < arguments.size() - 1) std::cout << ", ";
    }
    std::cout << ")" << std::endl;
    
    for (int i = 0; i < indent + 1; ++i) {
        std::cout << "  ";
    }
    std::cout << "Body: ";
    expr->print(indent + 2);
}

DefFuncNode::~DefFuncNode() {
    delete expr;
}
