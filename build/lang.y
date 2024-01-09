%{
#include <cstdio>
#include "RE.hpp"
#include "lang.hpp"
#include "lexer.hpp"
void yyerror(char * str);
int yylex();
GlobItemList * root;
%}

%union 
{
    long long n; // natural number
    char * i; // identifier
    Expr * e; // expression
    ExprList * el; // expression list
    Type * t; // type
    TypeList * tl; // typelist
    Cmd * c; // command
    Var * v; // var
    VarList * vl; // var list
    GlobItem * gi; // global item
    GlobItemList * gil; // global item list
    void * none; 
}

// terminals
%token <n> TM_NAT // natural number
%token <i> TM_IDENT // identifier
%token <none> TM_LEFT_BRACE TM_RIGHT_BRACE // {, }
%token <none> TM_LEFT_PAREN TM_RIGHT_PAREN // (, )
%token <none> TM_LT TM_LE TM_GT TM_GE TM_EQ TM_NE // <, <=, >, >=, ==, !=
%token <none> TM_PLUS TM_MINUS TM_MUL TM_DIV TM_MOD // +, -, *, /, %
%token <none> TM_SEMICOL TM_COMMA TM_ASGNOP // ;, ",", =
%token <none> TM_OR TM_AND TM_NOT // ||, &&, !
%token <none> TM_ADDR_OF // &
%token <none> TM_INT TM_VOID TM_IF TM_THEN TM_ELSE TM_WHILE TM_DO TM_FOR  // int, void, if, then, else, while, do, for
%token <none> TM_CON TM_BREAK TM_RET // continue, break, return
%token <none> TM_RI TM_WC TM_WI // read_int, write_char, write_int

// non-terminals
%type <e> NT_EXPR_U // one operator expression
%type <e> NT_EXPR // expression
%type <el> NT_EXPR_LST // expression list
%type <c> NT_CMD // command
%type <t> NT_TYPE // type
%type <tl> NT_TYPE_LST // type list
%type <gi> NT_GLB_ITM // global item
%type <v> NT_VAR_DECL // var declination
%type <vl> NT_VAR_LST // var list
%type <n> NT_STAR_LST // star list
%type <gil> NT_GLB_ITM_LST NT_PROGRAM // global item list

// priority
%nonassoc TM_ASGNOP 
%left TM_OR 
%left TM_AND 
%left TM_LT TM_LE TM_GT TM_GE TM_EQ TM_NE 
%left TM_PLUS TM_MINUS
%left TM_MUL TM_DIV TM_MOD
%left TM_NOT TM_ADDR_OF
%left TM_LEFT_PAREN TM_RIGHT_PAREN
%right TM_COMMA
%right TM_SEMICOL

%%
NT_PROGRAM:
    NT_GLB_ITM_LST
    {
        $$ = ($1);
        root = $$;
    }
;

NT_GLB_ITM_LST:
    NT_GLB_ITM
    {
        $$ = (TGCons($1, TGNil()));
    }
    |   NT_GLB_ITM TM_SEMICOL NT_GLB_ITM_LST
    {
        $$ = (TGCons($1, $3));
    }
;

NT_TYPE:
    TM_INT
    {
        $$ = (TTypeInt(0));
    }
    |   TM_VOID
    {
        $$ = (TTypeVoid(0));
    }
    |   TM_INT NT_STAR_LST
    {
        $$ = (TTypeInt($2));
    }
    |   NT_TYPE TM_LEFT_PAREN NT_STAR_LST TM_RIGHT_PAREN TM_LEFT_PAREN TM_RIGHT_PAREN
    {
        $$ = (TTypeFuncPtr($3, $1, TTypeNil()));
    }
    |   NT_TYPE TM_LEFT_PAREN NT_STAR_LST TM_RIGHT_PAREN TM_LEFT_PAREN NT_TYPE_LST TM_RIGHT_PAREN
    {
        $$ = (TTypeFuncPtr($3, $1, $6));
    }
;

NT_TYPE_LST:
    NT_TYPE
    {
        if (($1) -> type == T_VOID)
            throw RuntimeError("try to define a function pointer with a void as its argument");
        $$ = (TTypeListCons($1, TTypeNil()));
    }
    |   NT_TYPE TM_COMMA NT_TYPE_LST
    {
        if (($1) -> type == T_VOID)
            throw RuntimeError("try to define a function pointer with a void as its argument");
        $$ = (TTypeListCons($1, $3));
    }
;

NT_STAR_LST:
    TM_MUL
    {
        $$ = (1);
    }
    |   TM_MUL NT_STAR_LST
    {
        $$ = (($2) + 1);
    }
;

NT_VAR_DECL:
    NT_TYPE TM_IDENT
    {
        if (($1) -> type == T_VOID)
            throw RuntimeError("try to define a function with a void as its argument");
        $$ = (TVarDecl($1, $2));
    }
    |   NT_TYPE TM_LEFT_PAREN NT_STAR_LST TM_IDENT TM_RIGHT_PAREN TM_LEFT_PAREN TM_RIGHT_PAREN
    {
        $$ = (TVarDecl(TTypeFuncPtr($3, $1, TTypeNil()), $4));
    }
    |   NT_TYPE TM_LEFT_PAREN NT_STAR_LST TM_IDENT TM_RIGHT_PAREN TM_LEFT_PAREN NT_TYPE_LST TM_RIGHT_PAREN
    {
        $$ = (TVarDecl(TTypeFuncPtr($3, $1, $7), $4));
    }
;

NT_GLB_ITM:
    NT_VAR_DECL
    {
        $$ = (TGlobVar($1));
    }
    |   NT_TYPE TM_IDENT TM_LEFT_PAREN TM_RIGHT_PAREN TM_LEFT_BRACE NT_CMD TM_RIGHT_BRACE
    {
        $$ = (TFuncDef($1, $2, TVNil(), $6));
    }
    |   NT_TYPE TM_IDENT TM_LEFT_PAREN NT_VAR_LST TM_RIGHT_PAREN TM_LEFT_BRACE NT_CMD TM_RIGHT_BRACE
    {
        $$ = (TFuncDef($1, $2, $4, $7));
    }
;

NT_VAR_LST:
    NT_VAR_DECL
    {
        $$ = (TVCons($1, TVNil()));
    }
    |   NT_VAR_DECL TM_COMMA NT_VAR_LST
    {
        $$ = (TVCons($1, $3));
    }
;

NT_CMD:
    NT_VAR_DECL
    {
        $$ = (TDecl($1));
    }
    |   NT_EXPR TM_ASGNOP NT_EXPR
    {
        $$ = (TAsgn($1, $3));
    }
    |   NT_CMD TM_SEMICOL NT_CMD
    {
        $$ = (TSeq($1, $3));
    }
    |   TM_IF TM_LEFT_PAREN NT_EXPR TM_RIGHT_PAREN TM_THEN TM_LEFT_BRACE NT_CMD TM_RIGHT_BRACE TM_ELSE TM_LEFT_BRACE NT_CMD TM_RIGHT_BRACE
    {
        $$ = (TSeq(TNewFrame(), TSeq(TIf($3, $7, $11), TDelFrame())));
    } 
    |   TM_WHILE TM_LEFT_PAREN NT_EXPR TM_RIGHT_PAREN TM_DO TM_LEFT_BRACE NT_CMD TM_RIGHT_BRACE
    {
        $$ = (TWhile($3, TSeq(TNewFrame(), TSeq($7, TDelFrame()))));
    }
    |   TM_FOR TM_LEFT_BRACE NT_CMD TM_RIGHT_BRACE TM_LEFT_PAREN NT_EXPR TM_RIGHT_PAREN TM_LEFT_BRACE NT_CMD TM_RIGHT_BRACE TM_LEFT_BRACE NT_CMD TM_RIGHT_BRACE
    {
        $$ = (TSeq(TNewFrame(), TSeq(TFor($3, $6, $9, TSeq(TNewFrame(), TSeq($12, TDelFrame()))), TDelFrame())));
    }
    |   TM_DO TM_LEFT_BRACE NT_CMD TM_RIGHT_BRACE TM_WHILE TM_LEFT_PAREN NT_EXPR TM_RIGHT_PAREN
    {
        $$ = (TDoWhile(TSeq(TNewFrame(), TSeq($3, TDelFrame())), $7));
    }
    |   NT_EXPR TM_LEFT_PAREN TM_RIGHT_PAREN
    {
        $$ = (TProc($1, TENil()));
    }
    |   NT_EXPR TM_LEFT_PAREN NT_EXPR_LST TM_RIGHT_PAREN
    {
        $$ = (TProc($1, $3));
    }
    |   TM_WC TM_LEFT_PAREN NT_EXPR TM_RIGHT_PAREN
    {
        $$ = (TWriteChar($3));
    }
    |   TM_WI TM_LEFT_PAREN NT_EXPR TM_RIGHT_PAREN
    {
        $$ = (TWriteInt($3));
    }
    |   TM_CON
    {
        $$ = (TContinue());
    }
    |   TM_BREAK
    {
        $$ = (TBreak());
    }
    |   TM_RET
    {
        $$ = (TReturn());
    }
;

NT_EXPR_LST:
    NT_EXPR
    {
        $$ = (TECons($1, TENil()));
    }
    |   NT_EXPR TM_COMMA NT_EXPR_LST
    {
        $$ = (TECons($1, $3));
    }
;

NT_EXPR_U:
    TM_NAT
    {
        $$ = (TConst($1));
    }
    |   TM_LEFT_PAREN NT_EXPR TM_RIGHT_PAREN
    {
        $$ = ($2);
    }
    |   TM_IDENT
    {
        $$ = (TVar($1));
    }
    |   TM_MINUS NT_EXPR_U
    {
        $$ = (TUnOp(T_UMINUS, $2));
    }
    |   TM_MUL NT_EXPR_U
    {
        $$ = (TDeref($2));
    }
    |   TM_ADDR_OF NT_EXPR_U
    {
        $$ = (TAddrOf($2));
    }
    |   TM_NOT NT_EXPR_U
    {
        $$ = (TUnOp(T_NOT, $2));
    }
    |   TM_RI TM_LEFT_PAREN TM_RIGHT_PAREN
    {
        $$ = (TReadInt());
    }
    |   NT_EXPR_U TM_LEFT_PAREN TM_RIGHT_PAREN
    {
        $$ = (TFunc($1, TENil()));
    }
    |   NT_EXPR_U TM_LEFT_PAREN NT_EXPR_LST TM_RIGHT_PAREN
    {
        $$ = (TFunc($1, $3));
    }
;

NT_EXPR:
    NT_EXPR_U
    {
        $$ = ($1);
    }
    |   NT_EXPR TM_PLUS NT_EXPR
    {
        $$ = (TBinOp(T_PLUS, $1, $3));
    }
    |   NT_EXPR TM_MINUS NT_EXPR
    {
        $$ = (TBinOp(T_MINUS, $1, $3));
    }
    |   NT_EXPR TM_MUL NT_EXPR
    {
        $$ = (TBinOp(T_MUL, $1, $3));
    }
    |   NT_EXPR TM_DIV NT_EXPR
    {
        $$ = (TBinOp(T_DIV, $1, $3));
    }
    |   NT_EXPR TM_MOD NT_EXPR
    {
        $$ = (TBinOp(T_MOD, $1, $3));
    }
    |   NT_EXPR TM_LT NT_EXPR
    {
        $$ = (TBinOp(T_LT, $1, $3));
    }
    |   NT_EXPR TM_GT NT_EXPR
    {
        $$ = (TBinOp(T_GT, $1, $3));
    }
    |   NT_EXPR TM_LE NT_EXPR
    {
        $$ = (TBinOp(T_LE, $1, $3));
    }
    |   NT_EXPR TM_GE NT_EXPR
    {
        $$ = (TBinOp(T_GE, $1, $3));
    }
    |   NT_EXPR TM_EQ NT_EXPR
    {
        $$ = (TBinOp(T_EQ, $1, $3));
    }
    |   NT_EXPR TM_NE NT_EXPR
    {
        $$ = (TBinOp(T_NE, $1, $3));
    }
    |   NT_EXPR TM_AND NT_EXPR
    {
        $$ = (TBinOp(T_AND, $1, $3));
    }
    |   NT_EXPR TM_OR NT_EXPR
    {
        $$ = (TBinOp(T_OR, $1, $3));
    }
;

%%

void yyerror(char* s)
{
    fprintf(stderr , "%s\n", s);
}