%option noyywrap yylineno

%{
#include <iostream>
using namespace std;
%}

DIGIT      [0-9]
LETTER     [a-zA-Z_]
ID         {LETTER}({LETTER}|{DIGIT})*
INT        {DIGIT}+
FLOAT      ({DIGIT}+"."{DIGIT}+)

%%

[ \t\r\n]+         { /* ignorar espacios */ }


"//".*             { /* ignorar comentario */ }
\/\*([^*]|\*+[^*/])*\*+\/   { /* ignorar comentario bloque */ }


"fn"               { cout << "TOKEN: FN\n"; }
"let"              { cout << "TOKEN: LET\n"; }
"return"           { cout << "TOKEN: RETURN\n"; }
"if"               { cout << "TOKEN: IF\n"; }
"else"             { cout << "TOKEN: ELSE\n"; }
"while"            { cout << "TOKEN: WHILE\n"; }
"for"              { cout << "TOKEN: FOR\n"; }
"in"               { cout << "TOKEN: IN\n"; }


"i32"              { cout << "TOKEN: T_I32\n"; }
"f64"              { cout << "TOKEN: T_F64\n"; }
"bool"             { cout << "TOKEN: T_BOOL\n"; }
"char"             { cout << "TOKEN: T_CHAR\n"; }
"str"              { cout << "TOKEN: T_STR\n"; }


"true"             { cout << "TOKEN: TRUE\n"; }
"false"            { cout << "TOKEN: FALSE\n"; }


"->"               { cout << "TOKEN: ARROW\n"; }
"&&"               { cout << "TOKEN: AND\n"; }
"||"               { cout << "TOKEN: OR\n"; }
"=="               { cout << "TOKEN: EQ\n"; }
"!="               { cout << "TOKEN: NEQ\n"; }
"<="               { cout << "TOKEN: LE\n"; }
">="               { cout << "TOKEN: GE\n"; }
"="                { cout << "TOKEN: ASSIGN\n"; }
"<"                { cout << "TOKEN: LT\n"; }
">"                { cout << "TOKEN: GT\n"; }
"+"                { cout << "TOKEN: PLUS\n"; }
"-"                { cout << "TOKEN: MINUS\n"; }
"*"                { cout << "TOKEN: MUL\n"; }
"/"                { cout << "TOKEN: DIV\n"; }
"%"                { cout << "TOKEN: MOD\n"; }
"!"                { cout << "TOKEN: NOT\n"; }


"{"                { cout << "TOKEN: {\n"; }
"}"                { cout << "TOKEN: }\n"; }
"("                { cout << "TOKEN: (\n"; }
")"                { cout << "TOKEN: )\n"; }
"["                { cout << "TOKEN: [\n"; }
"]"                { cout << "TOKEN: ]\n"; }
","                { cout << "TOKEN: ,\n"; }
";"                { cout << "TOKEN: ;\n"; }
":"                { cout << "TOKEN: :\n"; }


\"([^\"\\]|\\.)*\" { cout << "TOKEN: STRING -> " << yytext << "\n"; }
\'([^\'\\]|\\.)\'  { cout << "TOKEN: CHAR -> " << yytext << "\n"; }
{FLOAT}            { cout << "TOKEN: FLOAT -> " << yytext << "\n"; }
{INT}              { cout << "TOKEN: INT -> " << yytext << "\n"; }


{ID}               { cout << "TOKEN: IDENT -> " << yytext << "\n"; }


. {
    cout << "ERROR LEXICO: " << yytext 
         << " en linea " << yylineno << "\n";
}

%%
