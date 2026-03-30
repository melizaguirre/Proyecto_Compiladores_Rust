#include <iostream>
#include <cstdio>
#include "ast.hpp"
#include "ast_printer.hpp"

extern int yyparse();
extern FILE* yyin;
extern ProgramNode* root;

int main(int argc, char* argv[]) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            std::cerr << "No se pudo abrir el archivo: " << argv[1] << "\n";
            return 1;
        }
    }

    if (yyparse() == 0) {
        std::cout << "\nSintaxis correcta\n\n";
        if (root) {
            ASTPrinter printer;
            printer.print(root);
        }
    }

    if (argc > 1 && yyin) {
        fclose(yyin);
    }

    return 0;
}