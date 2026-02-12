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


{WS}                
\n                  

/*Comentarios*/
"//".*              
/\*([^*]|\*+[^*/])*\*+/   

"i32"               { return T_I32; }
"f64"               { return T_F64; }
"bool"              { return T_BOOL; }
"char"              { return T_CHAR; }
"str"               { return T_STR; }

"fn"                { return FN; }
"let"               { return LET; }
"if"                { return IF; }
"else"              { return ELSE; }
"while"             { return WHILE; }
"for"               { return FOR; }
"in"                { return IN; }
"return"            { return RETURN; }

"true"              { return TRUE; }
"false"             { return FALSE; }

"->"                { return ARROW; }

"&&"                { return OP_AND; }
"||"                { return OP_OR; }

"=="                { return OP_EQ; }
"!="                { return OP_NEQ; }
"<="                { return OP_LE; }
">="                { return OP_GE; }