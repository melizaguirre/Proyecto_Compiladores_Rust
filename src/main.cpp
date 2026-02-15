#include <iostream>

extern int yylex();
extern FILE* yyin;

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Uso: ./lexer <archivo.txt>\n";
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("Error abriendo archivo");
        return 1;
    }

    std::cout << "=== INICIANDO ANALISIS LEXICO ===\n\n";

    while (yylex()) {
        // imprime los tokens
    }

    std::cout << "\n=== FIN DEL ANALISIS ===\n";

    fclose(yyin);
    return 0;
}
