#include <cstdio>
#include <iostream>

extern FILE* yyin;
extern int yyparse();

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Uso: ./rust_parser <archivo.txt>\n";
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("Error abriendo archivo");
        return 1;
    }

    int result = yyparse();

    fclose(yyin);

    if (result == 0) {
        std::cout << "Sintaxis correcta\n";
    }

    return result;
}