%option noyywrap nodefault yylineno

%{
#include "parser.tab.h"   
#include <cstdio>
#include <cstdlib>
#include <cstring>

%}

/* ===== Macros (patrones) ===== */
DIGIT      [0-9]
LETTER     [a-zA-Z_]
ID         {LETTER}({LETTER}|{DIGIT})*
INT        {DIGIT}+
FLOAT      ({DIGIT}+"."{DIGIT}+)
WS         [ \t\r]+

%%


{WS}                { /* ignore */ }
\n                  { /* yylineno se actualiza solo */ }

/*Comentarios*/
"//".*              { /* comentario de línea */ }
/\*([^*]|\*+[^*/])*\*+/   { /* comentario bloque */ }
