#ifndef AST_H
#define AST_H
#include <iostream>
#include <string>
#include <vector>

class ASTNode {
public:
    virtual void print(int indent) = 0;
    virtual ~ASTNode() {}
};

class NumberNode : public ASTNode {
public:
    double value;
    NumberNode(double val) : value(val) {}
    void print(int indent) override {
        for(int i=0; i<indent; i++) std::cout << "  ";
        std::cout << "Number: " << value << "\n";
    }
};

class IdentifierNode : public ASTNode {
public:
    std::string name;
    IdentifierNode(std::string n) : name(n) {}
    void print(int indent) override {
        for(int i=0; i<indent; i++) std::cout << "  ";
        std::cout << "Identifier: " << name << "\n";
    }
};

class BinaryOpNode : public ASTNode {
public:
    char op; ASTNode* left; ASTNode* right;
    BinaryOpNode(char op, ASTNode* l, ASTNode* r) : op(op), left(l), right(r) {}
    void print(int indent) override {
        for(int i=0; i<indent; i++) std::cout << "  ";
        std::cout << "BinaryOp: " << op << "\n";
        if(left) left->print(indent + 1);
        if(right) right->print(indent + 1);
    }
};

class AssignmentNode : public ASTNode {
public:
    std::string varName; ASTNode* expression;
    AssignmentNode(std::string name, ASTNode* expr) : varName(name), expression(expr) {}
    void print(int indent) override {
        for(int i=0; i<indent; i++) std::cout << "  ";
        std::cout << "Assignment: " << varName << "\n";
        if(expression) expression->print(indent + 1);
    }
};

class DeclarationNode : public ASTNode {
public:
    std::string type; std::string varName; ASTNode* initializer;
    DeclarationNode(std::string t, std::string name, ASTNode* init = nullptr) : type(t), varName(name), initializer(init) {}
    void print(int indent) override {
        for(int i=0; i<indent; i++) std::cout << "  ";
        std::cout << "Declaration: " << type << " " << varName << "\n";
        if(initializer) initializer->print(indent + 1);
    }
};

class ProgramNode : public ASTNode {
public:
    std::vector<ASTNode*> statements;
    void print(int indent) override {
        for(int i=0; i<indent; i++) std::cout << "  ";
        std::cout << "Program\n";
        for(auto stmt : statements) if(stmt) stmt->print(indent + 1);
    }
};
#endif