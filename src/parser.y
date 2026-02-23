%{
#include <cstdio>
#include <cstdlib>

int yylex();
extern int yylineno;
void yyerror(const char* s);
%}

%token FN LET IF ELSE WHILE FOR IN RETURN
%token T_I32 T_F64 T_BOOL T_CHAR T_STR
%token TRUE FALSE
%token ARROW
%token OP_AND OP_OR OP_NOT
%token OP_EQ OP_NEQ OP_LT OP_GT OP_LE OP_GE
%token OP_ASSIGN OP_ADD OP_SUB OP_MUL OP_DIV OP_MOD
%token OPEN_CURLY CLOSE_CURLY OPEN_PAR CLOSE_PAR
%token COMMA SEMICOLON COLON
%token IDENT
%token INT_CONST FLOAT_CONST STRING_LITERAL CHAR_LITERAL
%token ERROR

%start programa

%%