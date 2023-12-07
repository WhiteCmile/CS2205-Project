#ifndef LANG_H_INCLUDED
#define LANG_H_INCLUDED

#include <cstdio>
#include <cstdlib>
#include "lib.hpp"

enum BinOpType {
    T_PLUS, // +
    T_MINUS, // -
    T_MUL, // *
    T_DIV, // /
    T_MOD, // %
    T_LT, // <
    T_GT, // >
    T_LE, // <=
    T_GE, // >=
    T_EQ, // ==
    T_NE, // !=
    T_AND, // &&
    T_OR // ||
};

enum UnOpType {
    T_UMINUS, // -
    T_NOT // !
};

enum ExprType {
    T_CONST,
    T_VAR,
    T_BINOP,
    T_UNOP,
    T_DEREF,
    T_ADDR_OF,
    T_RI,
    T_FUNC
};

enum CmdType {
    T_DECL,
    T_ASGN,
    T_SEQ,
    T_IF,
    T_WHILE,
    T_FOR,
    T_DO_WHILE,
    T_PROC,
    T_WC,
    T_WI,
    T_BREAK,
    T_CONTINUE,
    T_RETURN
};

enum VarType {
    T_FUNCPTR,
    T_VOID,
    T_INT
};

enum GlobItemType {
    T_FUNC_DEF,
    T_GLOB_VAR
};

struct Expr;

struct ExprList {
    Expr * data;
    ExprList * next;
};

struct Expr{
    ExprType e_type;
    union {
        struct { long long val; } CONST;
        struct { char * name; } VAR;
        struct { BinOpType op; Expr * left; Expr * right; } BINOP;
        struct { UnOpType op; Expr * arg; } UNOP;
        struct { Expr * arg; } DEREF;
        struct { Expr * arg; } ADDR_OF;
        struct { void * none; } RI;
        struct { char * name; ExprList * args; } FUNC;
    } data;
};

struct Type;

struct TypeList;

struct Type {
    VarType type;
    int ptr_index;
    union {
        struct { void * none; } INT;
        struct { void * none; } VOID;
        struct { Type * ret_type; TypeList * list; } FUNCPTR;
    } data;
};

struct TypeList {
    Type * data;
    TypeList * next;
};

struct Var {
    Type * type;
    char * name;
};

struct Cmd {
    CmdType c_type;
    union {
        struct { Var * var; } DECL;
        struct { Expr * left; Expr * right; } ASGN;
        struct { Cmd * left; Cmd * right; } SEQ;
        struct { Expr * cond; Cmd * left; Cmd * right; } IF;
        struct { Expr * cond; Cmd * body; } WHILE;
        struct { Cmd * init; Expr * cond; Cmd * incr; Cmd * body; } FOR;
        struct { Cmd * body; Expr * cond; } DO_WHILE;
        struct { char * name; ExprList * args; } PROC;
        struct { Expr * expr; } WC;
        struct { Expr * expr; } WI;
        struct { void * none; } BREAK;
        struct { void * none; } CONTINUE;
        struct { void * none; } RETURN;
    } data;
};

struct VarList {
    Var * var;
    VarList * next;
};

struct GlobItem {
    GlobItemType type;
    union {
        struct { Type * ret_type; char * name; VarList * args; Cmd * body; } FUNC_DEF;
        struct { Var * var; } GLOB_VAR;
    } data;
};

struct GlobItemList {
    GlobItem * data;
    GlobItemList * next;
};

ExprList * TENil();
ExprList * TECons(Expr* data, ExprList * next);
Expr* TConst(long long val);
Expr* TVar(char * name);
Expr* TBinOp(BinOpType op, Expr* left,
                     Expr* right);
Expr * TUnOp(UnOpType op, Expr* arg);
Expr * TDeref(Expr* arg);
Expr * TAddrOf(Expr* arg);
Expr * TReadInt();
Expr * TFunc(char * name, ExprList * args);
Type * TTypeInt(int n);
Type * TTypeVoid(int n);
Type * TTypeFuncPtr(int n, Type * ret_type, TypeList * lst); 
TypeList * TTypeNil();
TypeList * TTypeListCons(Type * data, TypeList * next);
Cmd * TDecl(Var * var);
Cmd * TAsgn(Expr* left, Expr* right);
Cmd * TSeq(Cmd * left, Cmd * right);
Cmd * TIf(Expr* cond, Cmd * left, Cmd * right);
Cmd * TWhile(Expr* cond, Cmd * body);
Cmd * TFor(Cmd * init, Expr* cond,
                  Cmd * incr, Cmd * body);
Cmd * TDoWhile(Cmd * body, Expr* cond);
Cmd * TProc(char * name, ExprList * args);
Cmd * TLocalDecl(char * name, Cmd * body);
Cmd * TWriteChar(Expr * expr);
Cmd * TWriteInt(Expr * expr);
Cmd * TBreak();
Cmd * TContinue();
Cmd * TReturn();
Var * TVarDecl(Type * type, char * name);
VarList * TVNil();
VarList * TVCons(Var * var, VarList * lst);
GlobItem * TGlobVar(Var * var);
GlobItem * TFuncDef(Type * ret_type, char * name, VarList * args,
                            Cmd * body);
GlobItemList * TGNil();
GlobItemList * TGCons(GlobItem * data,
                               GlobItemList * next);

void PrintBinOp(BinOpType op);
void PrintUnOp(UnOpType op);
void PrintExpr(Expr * e);
void PrintExprList(ExprList * es);
void PrintType(Type * t);
void PrintTypeList(TypeList * l);
void PrintCmd(Cmd * c);
void PrintVar(Var * v);
void PrintVarList(VarList * vs);
void PrintGlobItem(GlobItem * g);
void PrintGlobItemList(GlobItemList * gs);

#endif // LANG_H_INCLUDED