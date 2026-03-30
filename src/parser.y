%define parse.error verbose

%{
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include "ast.hpp"

int yylex();
extern int yylineno;
extern char* yytext;

extern std::string last_token_text;
extern int last_token_line;

void yyerror(const char* s);

ProgramNode* root = nullptr;
%}

%union {
    char* str;
    ASTNode* node;
    ExprNode* expr;
    StmtNode* stmt;
    BlockNode* block;
    TypeNode* type;
    ParamNode* param;
    FunctionNode* func;
    ProgramNode* program;
    std::vector<ExprNode*>* expr_list;
    std::vector<StmtNode*>* stmt_list;
    std::vector<ParamNode*>* param_list;
    std::vector<FunctionNode*>* func_list;
}

%token FN LET IF ELSE WHILE FOR IN RETURN
%token T_I32 T_F64 T_BOOL T_CHAR T_STR
%token TRUE FALSE
%token OPEN_PAR CLOSE_PAR
%token OPEN_CURLY CLOSE_CURLY
%token OPEN_BRACKET CLOSE_BRACKET
%token COMMA SEMICOLON COLON ARROW
%token OP_AND OP_OR OP_NOT
%token OP_EQ OP_NEQ OP_LT OP_GT OP_LE OP_GE
%token OP_ASSIGN OP_ADD OP_SUB OP_MUL OP_DIV OP_MOD
%token ERROR

%token <str> IDENT INT_CONST FLOAT_CONST STRING_LITERAL CHAR_LITERAL

%type <program> programa
%type <func_list> funciones
%type <func> funcion
%type <param_list> param_opt param_tail
%type <param> parametro
%type <type> tipo ret_opt let_tipo_opt
%type <block> bloque
%type <stmt_list> sentencias
%type <stmt> sentencia let_stmt id_stmt return_stmt if_sentencia while_sentencia for_sentencia else_opcional
%type <expr> let_init_opt expr_opt expresion and_expr igualdad relacional aditiva multiplicativa unaria primaria literal
%type <expr_list> argumentos_opt argumentos_prima

%start programa

%%

programa
  : funciones
    {
      $$ = new ProgramNode();
      $$->functions = *$1;
      root = $$;
    }
  ;

funciones
  : /* empty */
    {
      $$ = new std::vector<FunctionNode*>();
    }
  | funcion funciones
    {
      $$ = $2;
      $$->insert($$->begin(), $1);
    }
  ;

funcion
  : FN IDENT OPEN_PAR param_opt CLOSE_PAR ret_opt bloque
    {
      $$ = new FunctionNode($2, *$4, $6, $7);
    }
  ;

param_opt
  : /* empty */
    {
      $$ = new std::vector<ParamNode*>();
    }
  | parametro param_tail
    {
      $$ = $2;
      $$->insert($$->begin(), $1);
    }
  ;

param_tail
  : /* empty */
    {
      $$ = new std::vector<ParamNode*>();
    }
  | COMMA parametro param_tail
    {
      $$ = $3;
      $$->insert($$->begin(), $2);
    }
  ;

parametro
  : IDENT COLON tipo
    {
      $$ = new ParamNode($1, $3);
    }
  ;

ret_opt
  : /* empty */
    {
      $$ = nullptr;
    }
  | ARROW tipo
    {
      $$ = $2;
    }
  ;

tipo
  : T_I32   { $$ = new TypeNode("i32"); }
  | T_F64   { $$ = new TypeNode("f64"); }
  | T_BOOL  { $$ = new TypeNode("bool"); }
  | T_CHAR  { $$ = new TypeNode("char"); }
  | T_STR   { $$ = new TypeNode("str"); }
  ;

bloque
  : OPEN_CURLY sentencias CLOSE_CURLY
    {
      $$ = new BlockNode();
      $$->statements = *$2;
    }
  ;

sentencias
  : /* empty */
    {
      $$ = new std::vector<StmtNode*>();
    }
  | sentencia sentencias
    {
      $$ = $2;
      $$->insert($$->begin(), $1);
    }
  ;

sentencia
  : let_stmt SEMICOLON       { $$ = $1; }
  | id_stmt SEMICOLON        { $$ = $1; }
  | return_stmt SEMICOLON    { $$ = $1; }
  | if_sentencia             { $$ = $1; }
  | while_sentencia          { $$ = $1; }
  | for_sentencia            { $$ = $1; }
  | bloque                   { $$ = $1; }
  ;

let_stmt
  : LET IDENT let_tipo_opt let_init_opt
    {
      $$ = new LetStmtNode($2, $3, $4);
    }
  ;

let_tipo_opt
  : /* empty */
    {
      $$ = nullptr;
    }
  | COLON tipo
    {
      $$ = $2;
    }
  ;

let_init_opt
  : /* empty */
    {
      $$ = nullptr;
    }
  | OP_ASSIGN expresion
    {
      $$ = $2;
    }
  ;

id_stmt
  : IDENT OP_ASSIGN expresion
    {
      $$ = new AssignStmtNode($1, $3);
    }
  | IDENT OPEN_PAR argumentos_opt CLOSE_PAR
    {
      $$ = new ExprStmtNode(new CallExprNode($1, *$3));
    }
  ;

return_stmt
  : RETURN expr_opt
    {
      $$ = new ReturnStmtNode($2);
    }
  ;

expr_opt
  : /* empty */
    {
      $$ = nullptr;
    }
  | expresion
    {
      $$ = $1;
    }
  ;

if_sentencia
  : IF expresion bloque else_opcional
    {
      $$ = new IfStmtNode($2, $3, $4);
    }
  ;

else_opcional
  : /* empty */
    {
      $$ = nullptr;
    }
  | ELSE bloque
    {
      $$ = $2;
    }
  | ELSE if_sentencia
    {
      $$ = $2;
    }
  ;

while_sentencia
  : WHILE expresion bloque
    {
      $$ = new WhileStmtNode($2, $3);
    }
  ;

for_sentencia
  : FOR IDENT IN expresion bloque
    {
      $$ = new ForStmtNode($2, $4, $5);
    }
  ;

argumentos_opt
  : /* empty */
    {
      $$ = new std::vector<ExprNode*>();
    }
  | expresion argumentos_prima
    {
      $$ = $2;
      $$->insert($$->begin(), $1);
    }
  ;

argumentos_prima
  : /* empty */
    {
      $$ = new std::vector<ExprNode*>();
    }
  | COMMA expresion argumentos_prima
    {
      $$ = $3;
      $$->insert($$->begin(), $2);
    }
  ;

expresion
  : expresion OP_OR and_expr
    {
      $$ = new BinaryExprNode($1, "||", $3);
    }
  | and_expr
    {
      $$ = $1;
    }
  ;

and_expr
  : and_expr OP_AND igualdad
    {
      $$ = new BinaryExprNode($1, "&&", $3);
    }
  | igualdad
    {
      $$ = $1;
    }
  ;

igualdad
  : igualdad OP_EQ relacional
    {
      $$ = new BinaryExprNode($1, "==", $3);
    }
  | igualdad OP_NEQ relacional
    {
      $$ = new BinaryExprNode($1, "!=", $3);
    }
  | relacional
    {
      $$ = $1;
    }
  ;

relacional
  : relacional OP_LT aditiva
    {
      $$ = new BinaryExprNode($1, "<", $3);
    }
  | relacional OP_GT aditiva
    {
      $$ = new BinaryExprNode($1, ">", $3);
    }
  | relacional OP_LE aditiva
    {
      $$ = new BinaryExprNode($1, "<=", $3);
    }
  | relacional OP_GE aditiva
    {
      $$ = new BinaryExprNode($1, ">=", $3);
    }
  | aditiva
    {
      $$ = $1;
    }
  ;

aditiva
  : aditiva OP_ADD multiplicativa
    {
      $$ = new BinaryExprNode($1, "+", $3);
    }
  | aditiva OP_SUB multiplicativa
    {
      $$ = new BinaryExprNode($1, "-", $3);
    }
  | multiplicativa
    {
      $$ = $1;
    }
  ;

multiplicativa
  : multiplicativa OP_MUL unaria
    {
      $$ = new BinaryExprNode($1, "*", $3);
    }
  | multiplicativa OP_DIV unaria
    {
      $$ = new BinaryExprNode($1, "/", $3);
    }
  | multiplicativa OP_MOD unaria
    {
      $$ = new BinaryExprNode($1, "%", $3);
    }
  | unaria
    {
      $$ = $1;
    }
  ;

unaria
  : OP_NOT unaria
    {
      $$ = new UnaryExprNode("!", $2);
    }
  | OP_SUB unaria
    {
      $$ = new UnaryExprNode("-", $2);
    }
  | OP_ADD unaria
    {
      $$ = new UnaryExprNode("+", $2);
    }
  | primaria
    {
      $$ = $1;
    }
  ;

primaria
  : literal
    {
      $$ = $1;
    }
  | IDENT
    {
      $$ = new IdentifierNode($1);
    }
  | IDENT OPEN_PAR argumentos_opt CLOSE_PAR
    {
      $$ = new CallExprNode($1, *$3);
    }
  | OPEN_PAR expresion CLOSE_PAR
    {
      $$ = $2;
    }
  ;

literal
  : INT_CONST
    {
      $$ = new LiteralNode($1);
    }
  | FLOAT_CONST
    {
      $$ = new LiteralNode($1);
    }
  | STRING_LITERAL
    {
      $$ = new LiteralNode($1);
    }
  | CHAR_LITERAL
    {
      $$ = new LiteralNode($1);
    }
  | TRUE
    {
      $$ = new LiteralNode("true");
    }
  | FALSE
    {
      $$ = new LiteralNode("false");
    }
  ;

%%

void yyerror(const char* s) {
    std::fprintf(stderr,
        "Error de sintaxis en linea %d: %s. Found: '%s'\n",
        last_token_line,
        s,
        last_token_text.empty() ? "EOF" : last_token_text.c_str());
}