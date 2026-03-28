#pragma once
#include <iostream>
#include "ast.hpp"

class ASTPrinter {
    int indent = 0;

    void tabs() {
        for (int i = 0; i < indent; i++) std::cout << "  ";
    }

public:
    void print(ASTNode* node) {
        if (!node) {
            tabs(); std::cout << "null\n";
            return;
        }

        if (auto p = dynamic_cast<ProgramNode*>(node)) {
            std::cout << "Program\n";
            indent++;
            for (auto f : p->functions) print(f);
            indent--;
        }
        else if (auto f = dynamic_cast<FunctionNode*>(node)) {
            tabs(); std::cout << "Function: " << f->name << "\n";
            indent++;
            tabs(); std::cout << "Params\n";
            indent++;
            for (auto param : f->params) print(param);
            indent--;
            tabs(); std::cout << "ReturnType: " 
                              << (f->returnType ? f->returnType->name : "void") << "\n";
            print(f->body);
            indent--;
        }
        else if (auto p = dynamic_cast<ParamNode*>(node)) {
            tabs(); std::cout << "Param: " << p->name
                              << " : " << (p->type ? p->type->name : "?") << "\n";
        }
        else if (auto b = dynamic_cast<BlockNode*>(node)) {
            tabs(); std::cout << "Block\n";
            indent++;
            for (auto s : b->statements) print(s);
            indent--;
        }
        else if (auto s = dynamic_cast<LetStmtNode*>(node)) {
            tabs(); std::cout << "LetStmt: " << s->name << "\n";
            indent++;
            tabs(); std::cout << "Type: " << (s->type ? s->type->name : "infer") << "\n";
            if (s->init) {
                tabs(); std::cout << "Init:\n";
                indent++;
                print(s->init);
                indent--;
            }
            indent--;
        }
        else if (auto s = dynamic_cast<AssignStmtNode*>(node)) {
            tabs(); std::cout << "AssignStmt: " << s->name << "\n";
            indent++;
            print(s->expr);
            indent--;
        }
        else if (auto s = dynamic_cast<ExprStmtNode*>(node)) {
            tabs(); std::cout << "ExprStmt\n";
            indent++;
            print(s->expr);
            indent--;
        }
        else if (auto s = dynamic_cast<ReturnStmtNode*>(node)) {
            tabs(); std::cout << "ReturnStmt\n";
            if (s->expr) {
                indent++;
                print(s->expr);
                indent--;
            }
        }
        else if (auto s = dynamic_cast<IfStmtNode*>(node)) {
            tabs(); std::cout << "IfStmt\n";
            indent++;
            tabs(); std::cout << "Condition:\n";
            indent++;
            print(s->condition);
            indent--;
            tabs(); std::cout << "Then:\n";
            indent++;
            print(s->thenBlock);
            indent--;
            if (s->elseBranch) {
                tabs(); std::cout << "Else:\n";
                indent++;
                print(s->elseBranch);
                indent--;
            }
            indent--;
        }
        else if (auto s = dynamic_cast<WhileStmtNode*>(node)) {
            tabs(); std::cout << "WhileStmt\n";
            indent++;
            tabs(); std::cout << "Condition:\n";
            indent++;
            print(s->condition);
            indent--;
            tabs(); std::cout << "Body:\n";
            indent++;
            print(s->body);
            indent--;
            indent--;
        }
        else if (auto s = dynamic_cast<ForStmtNode*>(node)) {
            tabs(); std::cout << "ForStmt: " << s->var << "\n";
            indent++;
            tabs(); std::cout << "Iterable:\n";
            indent++;
            print(s->iterable);
            indent--;
            tabs(); std::cout << "Body:\n";
            indent++;
            print(s->body);
            indent--;
            indent--;
        }
        else if (auto e = dynamic_cast<IdentifierNode*>(node)) {
            tabs(); std::cout << "Identifier: " << e->name << "\n";
        }
        else if (auto e = dynamic_cast<LiteralNode*>(node)) {
            tabs(); std::cout << "Literal: " << e->value << "\n";
        }
        else if (auto e = dynamic_cast<UnaryExprNode*>(node)) {
            tabs(); std::cout << "UnaryExpr: " << e->op << "\n";
            indent++;
            print(e->expr);
            indent--;
        }
        else if (auto e = dynamic_cast<BinaryExprNode*>(node)) {
            tabs(); std::cout << "BinaryExpr: " << e->op << "\n";
            indent++;
            print(e->left);
            print(e->right);
            indent--;
        }
        else if (auto e = dynamic_cast<CallExprNode*>(node)) {
            tabs(); std::cout << "CallExpr: " << e->callee << "\n";
            indent++;
            for (auto a : e->args) print(a);
            indent--;
        }
        else if (auto t = dynamic_cast<TypeNode*>(node)) {
            tabs(); std::cout << "Type: " << t->name << "\n";
        }
    }
};