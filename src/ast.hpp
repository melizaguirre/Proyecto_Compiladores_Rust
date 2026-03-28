#pragma once
#include <string>
#include <vector>

struct ASTNode {
    virtual ~ASTNode() = default;
};

struct ExprNode : ASTNode {
};

struct StmtNode : ASTNode {
};

struct TypeNode : ASTNode {
    std::string name;
    TypeNode(const std::string& n) : name(n) {}
};

struct LiteralNode : ExprNode {
    std::string value;
    LiteralNode(const std::string& v) : value(v) {}
};

struct IdentifierNode : ExprNode {
    std::string name;
    IdentifierNode(const std::string& n) : name(n) {}
};

struct UnaryExprNode : ExprNode {
    std::string op;
    ExprNode* expr;
    UnaryExprNode(const std::string& o, ExprNode* e) : op(o), expr(e) {}
};

struct BinaryExprNode : ExprNode {
    std::string op;
    ExprNode* left;
    ExprNode* right;
    BinaryExprNode(ExprNode* l, const std::string& o, ExprNode* r)
        : op(o), left(l), right(r) {}
};

struct CallExprNode : ExprNode {
    std::string callee;
    std::vector<ExprNode*> args;
    CallExprNode(const std::string& c, const std::vector<ExprNode*>& a)
        : callee(c), args(a) {}
};

struct LetStmtNode : StmtNode {
    std::string name;
    TypeNode* type;
    ExprNode* init;
    LetStmtNode(const std::string& n, TypeNode* t, ExprNode* i)
        : name(n), type(t), init(i) {}
};

struct AssignStmtNode : StmtNode {
    std::string name;
    ExprNode* expr;
    AssignStmtNode(const std::string& n, ExprNode* e)
        : name(n), expr(e) {}
};

struct ExprStmtNode : StmtNode {
    ExprNode* expr;
    ExprStmtNode(ExprNode* e) : expr(e) {}
};

struct ReturnStmtNode : StmtNode {
    ExprNode* expr;
    ReturnStmtNode(ExprNode* e) : expr(e) {}
};

struct BlockNode : StmtNode {
    std::vector<StmtNode*> statements;
};

struct IfStmtNode : StmtNode {
    ExprNode* condition;
    BlockNode* thenBlock;
    StmtNode* elseBranch; // puede ser BlockNode o IfStmtNode o nullptr
    IfStmtNode(ExprNode* c, BlockNode* t, StmtNode* e)
        : condition(c), thenBlock(t), elseBranch(e) {}
};

struct WhileStmtNode : StmtNode {
    ExprNode* condition;
    BlockNode* body;
    WhileStmtNode(ExprNode* c, BlockNode* b) : condition(c), body(b) {}
};

struct ForStmtNode : StmtNode {
    std::string var;
    ExprNode* iterable;
    BlockNode* body;
    ForStmtNode(const std::string& v, ExprNode* i, BlockNode* b)
        : var(v), iterable(i), body(b) {}
};

struct ParamNode : ASTNode {
    std::string name;
    TypeNode* type;
    ParamNode(const std::string& n, TypeNode* t) : name(n), type(t) {}
};

struct FunctionNode : ASTNode {
    std::string name;
    std::vector<ParamNode*> params;
    TypeNode* returnType;
    BlockNode* body;

    FunctionNode(const std::string& n,
                 const std::vector<ParamNode*>& p,
                 TypeNode* r,
                 BlockNode* b)
        : name(n), params(p), returnType(r), body(b) {}
};

struct ProgramNode : ASTNode {
    std::vector<FunctionNode*> functions;
};