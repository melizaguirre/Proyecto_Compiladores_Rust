

Este proyecto implementa un analizador léxico y sintáctico para un lenguaje inspirado en Rust.

El sistema es capaz de:

Identificar tokens mediante un lexer (Flex)
Validar la sintaxis mediante un parser (Bison)
Detectar errores sintácticos
Mostrar los tokens reconocidos
Generar una representación del AST (Abstract Syntax Tree)

---

## Estructura del Proyecto

* `src/` → Código principal (lexer, parser, AST)
* `gui/` → Interfaz gráfica (Qt)
* `tests/` → Archivos de prueba
* `CMakeLists.txt` → Configuración de compilación con CMake
* `Makefile` → Compilación rápida



Linux / MacOS / Windows
g++
Flex
Bison
CMake (opcional)
Qt (solo para interfaz gráfica)



```bash
make

./rust_parser tests/prueba1.txt


