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

programa
  : funciones
  ;

funciones
  : /* empty */
  | funcion funciones
  ;

funcion
  : FN IDENT OPEN_PAR param_opt CLOSE_PAR ret_opt bloque
  ;

param_opt
  : /* empty */
  | parametro param_tail
  ;

param_tail
  : /* empty */
  | COMMA parametro param_tail
  ;

parametro
  : IDENT COLON tipo
  ;

ret_opt
  : /* empty */
  | ARROW tipo
  ;

tipo
  : T_I32
  | T_F64
  | T_BOOL
  | T_CHAR
  | T_STR
  ;

bloque
  : OPEN_CURLY sentencias CLOSE_CURLY
  ;

sentencias
  : /* empty */
  | sentencia sentencias
  ;

sentencia
  : let_stmt SEMICOLON
  | id_stmt SEMICOLON
  | return_stmt SEMICOLON
  | if_sentencia
  | while_sentencia
  | for_sentencia
  | bloque
  ;

let_stmt
  : LET IDENT let_tipo_opt let_init_opt
  ;

let_tipo_opt
  : /* empty */
  | COLON tipo
  ;

let_init_opt
  : /* empty */
  | OP_ASSIGN expresion
  ;

id_stmt
  : IDENT id_stmt_tail
  ;

id_stmt_tail
  : OP_ASSIGN expresion
  | OPEN_PAR argumentos_opt CLOSE_PAR
  ;

return_stmt
  : RETURN expr_opt
  ;


expr_opt
  : /* empty */
  | expresion
  ;

if_sentencia
  : IF expresion bloque else_opcional
  ;

else_opcional
  : /* empty */
  | ELSE bloque
  | ELSE if_sentencia
  ;

while_sentencia
  : WHILE expresion bloque
  ; 

for_sentencia
  : FOR IDENT IN expresion bloque
  ;