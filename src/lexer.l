%option noyywrap yylineno nodefault

%{
#include "parser.tab.h"
#include <iostream>
#include <string>

using namespace std;

string last_token_text;
int last_token_line = 1;

static int emit(int tok, const char* name){
    last_token_text = yytext;
    last_token_line = yylineno;

    cout << "TOKEN: " << name;
    if(tok == IDENT || tok == INT_CONST || tok == FLOAT_CONST || tok == STRING_LITERAL || tok == CHAR_LITERAL)
        cout << " -> " << yytext;
    cout << "\n";

    return tok;
}
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

"fn"               { return emit(FN, "FN"); }
"let"              { return emit(LET, "LET"); }
"return"           { return emit(RETURN, "RETURN"); }
"if"               { return emit(IF, "IF"); }
"else"             { return emit(ELSE, "ELSE"); }
"while"            { return emit(WHILE, "WHILE"); }
"for"              { return emit(FOR, "FOR"); }
"in"               { return emit(IN, "IN"); }

"i32"              { return emit(T_I32, "T_I32"); }
"f64"              { return emit(T_F64, "T_F64"); }
"bool"             { return emit(T_BOOL, "T_BOOL"); }
"char"             { return emit(T_CHAR, "T_CHAR"); }
"str"              { return emit(T_STR, "T_STR"); }

"true"             { return emit(TRUE, "TRUE"); }
"false"            { return emit(FALSE, "FALSE"); }

"->"               { return emit(ARROW, "ARROW"); }
"&&"               { return emit(OP_AND, "AND"); }
"||"               { return emit(OP_OR, "OR"); }
"=="               { return emit(OP_EQ, "EQ"); }
"!="               { return emit(OP_NEQ, "NEQ"); }
"<="               { return emit(OP_LE, "LE"); }
">="               { return emit(OP_GE, "GE"); }
"="                { return emit(OP_ASSIGN, "ASSIGN"); }
"<"                { return emit(OP_LT, "LT"); }
">"                { return emit(OP_GT, "GT"); }
"+"                { return emit(OP_ADD, "PLUS"); }
"-"                { return emit(OP_SUB, "MINUS"); }
"*"                { return emit(OP_MUL, "MUL"); }
"/"                { return emit(OP_DIV, "DIV"); }
"%"                { return emit(OP_MOD, "MOD"); }
"!"                { return emit(OP_NOT, "NOT"); }

"{"                { return emit(OPEN_CURLY, "{"); }
"}"                { return emit(CLOSE_CURLY, "}"); }
"("                { return emit(OPEN_PAR, "("); }
")"                { return emit(CLOSE_PAR, ")"); }
"["                { return emit(OPEN_BRACKET, "["); }
"]"                { return emit(CLOSE_BRACKET, "]"); }
","                { return emit(COMMA, ","); }
";"                { return emit(SEMICOLON, ";"); }

":"                { return emit(COLON, ":"); }

\"([^\"\\]|\\.)*\" { return emit(STRING_LITERAL, "STRING"); }
\'([^\'\\]|\\.)\'  { return emit(CHAR_LITERAL, "CHAR"); }
{FLOAT}            { return emit(FLOAT_CONST, "FLOAT"); }
{INT}              { return emit(INT_CONST, "INT"); }

{ID}               { return emit(IDENT, "IDENT"); }

. {
    cout << "ERROR LEXICO: " << yytext << " en linea " << yylineno << "\n";
    return ERROR;
}

%%