#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "lang.hpp"

template <typename T>
T * NewPtr()
{
    T * res = new T;
    if (res == nullptr) 
    {
        puts("Failed in malloc");
        exit(0);
    }
    return res;
}

ExprList * TENil() {
    return nullptr;
}

ExprList * TECons(Expr * data, ExprList * next) {
    ExprList * res = NewPtr<ExprList> ();
    res -> data = data;
    res -> next = next;
    return res;
}

Expr * TConst(long long val) {
    Expr * res = NewPtr<Expr> ();
    res -> e_type = T_CONST;
    res -> data.CONST.val = val;
    return res;
}

Expr * TVar(char * name) {
    Expr * res = NewPtr<Expr> ();
    res -> e_type = T_VAR;
    res -> data.VAR.name = name;
    return res;
}

Expr * TBinOp(BinOpType op, Expr * left, Expr * right) {
    Expr * res = NewPtr<Expr> ();
    res -> e_type = T_BINOP;
    res -> data.BINOP.op = op;
    res -> data.BINOP.left = left;
    res -> data.BINOP.right = right;
    return res;
}

Expr * TUnOp(enum UnOpType op, Expr * arg) {
    Expr * res = NewPtr<Expr> ();
    res -> e_type = T_UNOP;
    res -> data.UNOP.op = op;
    res -> data.UNOP.arg = arg;
    return res;
}

Expr * TDeref(Expr * arg) {
    Expr * res = NewPtr<Expr> ();
    res -> e_type = T_DEREF;
    res -> data.DEREF.arg = arg;
    return res;
}

Expr * TAddrOf(Expr * arg) {
    Expr * res = NewPtr<Expr> ();
    res -> e_type = T_ADDR_OF;
    res -> data.ADDR_OF.arg = arg;
    return res;
}

Expr * TReadInt()
{
    Expr * res = NewPtr<Expr> ();
    res -> e_type = T_RI;
    return res;
}

Expr * TFunc(char * name, ExprList * args) {
    Expr * res = NewPtr<Expr> ();
    res -> e_type = T_FUNC;
    res -> data.FUNC.name = name;
    res -> data.FUNC.args = args;
    return res;
}

Type * TTypeInt(int n)
{
    Type * res = NewPtr<Type> ();
    res -> type = T_INT;
    res -> ptr_index = n;
    return res;
}

Type * TTypeVoid(int n)
{
    Type * res = NewPtr<Type> ();
    res -> type = T_VOID;
    res -> ptr_index = n;
    return res;
}

Type * TTypeFuncPtr(int n, Type * ret_type, TypeList * lst)
{
    Type * res = NewPtr<Type> ();
    res -> type = T_FUNCPTR;
    res -> ptr_index = n;
    res -> data.FUNCPTR.ret_type = ret_type;
    res -> data.FUNCPTR.list = lst;
    return res;
}

TypeList * TTypeNil()
{
    return nullptr;
}

TypeList * TTypeListCons(Type * data, TypeList * next)
{
    TypeList * res = NewPtr<TypeList> ();
    res -> data = data;
    res -> next = next;
    return res;
}

Cmd * TDecl(Var * var) {
    Cmd * res = NewPtr<Cmd> ();
    res -> c_type = T_DECL;
    res -> data.DECL.var = var;
    return res;
}

Cmd * TAsgn(Expr * left, Expr * right) {
    Cmd * res = NewPtr<Cmd> ();
    res -> c_type = T_ASGN;
    res -> data.ASGN.left = left;
    res -> data.ASGN.right = right;
    return res;
}

Cmd * TSeq(Cmd * left, Cmd * right) {
    Cmd * res = NewPtr<Cmd> ();
    res -> c_type = T_SEQ;
    res -> data.SEQ.left = left;
    res -> data.SEQ.right = right;
    return res;
}

Cmd * TIf(Expr * cond, Cmd * left, Cmd * right) {
    Cmd * res = NewPtr<Cmd> ();
    res -> c_type = T_IF;
    res -> data.IF.cond = cond;
    res -> data.IF.left = left;
    res -> data.IF.right = right;
    return res;
}

Cmd * TWhile(Expr * cond, Cmd * body) {
    Cmd * res = NewPtr<Cmd> ();
    res -> c_type = T_WHILE;
    res -> data.WHILE.cond = cond;
    res -> data.WHILE.body = body;
    return res;
}

Cmd * TFor(Cmd * init, Expr * cond, Cmd * incr, Cmd * body) {
    Cmd * res = NewPtr<Cmd> ();
    res -> c_type = T_FOR;
    res -> data.FOR.init = init;
    res -> data.FOR.cond = cond;
    res -> data.FOR.incr = incr;
    res -> data.FOR.body = body;
    return res;
}

Cmd * TDoWhile(Cmd * body, Expr * cond) {
    Cmd * res = NewPtr<Cmd> ();
    res -> c_type = T_DO_WHILE;
    res -> data.DO_WHILE.body = body;
    res -> data.DO_WHILE.cond = cond;
    return res;
}

Cmd * TProc(char * name, ExprList * args) {
    Cmd * res = NewPtr<Cmd> ();
    res -> c_type = T_PROC;
    res -> data.PROC.name = name;
    res -> data.PROC.args = args;
    return res;
}

Cmd * TWriteChar(Expr * expr)
{
    Cmd * res = NewPtr<Cmd> ();
    res -> c_type = T_WC;
    res -> data.WC.expr = expr;
    return res;
}

Cmd * TWriteInt(Expr * expr)
{
    Cmd * res = NewPtr<Cmd> ();
    res -> c_type = T_WI;
    res -> data.WI.expr = expr;
    return res;
}

Cmd * TBreak() {
    Cmd * res = NewPtr<Cmd> ();
    res -> c_type = T_BREAK;
    return res;
}

Cmd * TContinue() {
    Cmd * res = NewPtr<Cmd> ();
    res -> c_type = T_CONTINUE;
    return res;
}

Cmd * TReturn() {
    Cmd * res = NewPtr<Cmd> ();
    res -> c_type = T_RETURN;
    return res;
}

Var * TVarDecl(Type * type, char * name)
{
    Var * res = NewPtr<Var> ();
    res -> type = type, res -> name = name;
    return res;
}

VarList * TVNil() {
    return nullptr;
}

VarList * TVCons(Var * var, VarList * next) {
    VarList * res = NewPtr<VarList> ();
    res -> var = var;
    res -> next = next;
    return res;
}

GlobItem * TGlobVar(Var * var)
{
    GlobItem * res = NewPtr<GlobItem> ();
    res -> type = T_GLOB_VAR;
    res -> data.GLOB_VAR.var = var;
    return res;
}

GlobItem * TFuncDef(Type * ret_type, char * name, VarList * args, Cmd * body) {
    GlobItem * res = NewPtr<GlobItem> ();
    res -> type = T_FUNC_DEF;
    res -> data.FUNC_DEF.ret_type = ret_type;
    res -> data.FUNC_DEF.name = name;
    res -> data.FUNC_DEF.args = args;
    res -> data.FUNC_DEF.body = body;
    return res;
}

GlobItemList * TGNil() {
    return nullptr;
}

GlobItemList * TGCons(GlobItem * data, GlobItemList * next) {
    GlobItemList * res = NewPtr<GlobItemList> ();
    res -> data = data;
    res -> next = next;
    return res;
}

void PrintBinOp(BinOpType op) {
    switch (op) {
    case T_PLUS:
        printf("PLUS");
        break;
    case T_MINUS:
        printf("MINUS");
        break;
    case T_MUL:
        printf("MUL");
        break;
    case T_DIV:
        printf("DIV");
        break;
    case T_MOD:
        printf("MOD");
        break;
    case T_LT:
        printf("LT");
        break;
    case T_GT:
        printf("GT");
        break;
    case T_LE:
        printf("LE");
        break;
    case T_GE:
        printf("GE");
        break;
    case T_EQ:
        printf("EQ");
        break;
    case T_NE:
        printf("NE");
        break;
    case T_AND:
        printf("AND");
        break;
    case T_OR:
        printf("OR");
        break;
    }
}

void PrintUnOp(UnOpType op) {
    switch (op) {
    case T_UMINUS:
        printf("UMINUS");
        break;
    case T_NOT:
        printf("NOT");
        break;
    }
}

void PrintExpr(Expr * e) {
    switch (e -> e_type) {
    case T_CONST:
        printf("CONST(%lld)", e -> data.CONST.val);
        break;
    case T_VAR:
        printf("VAR(%s)", e -> data.VAR.name);
        break;
    case T_BINOP:
        PrintBinOp(e -> data.BINOP.op);
        printf("(");
        PrintExpr(e -> data.BINOP.left);
        printf(",");
        PrintExpr(e -> data.BINOP.right);
        printf(")");
        break;
    case T_UNOP:
        PrintUnOp(e -> data.UNOP.op);
        printf("(");
        PrintExpr(e -> data.UNOP.arg);
        printf(")");
        break;
    case T_DEREF:
        printf("DEREF(");
        PrintExpr(e -> data.DEREF.arg);
        printf(")");
        break;
    case T_ADDR_OF:
        printf("ADDR_OF(");
        PrintExpr(e -> data.ADDR_OF.arg);
        printf(")");
        break;
    case T_RI:
        printf("READ_INT()");
        break;
    case T_FUNC:
        printf("FUNC(%s", e -> data.FUNC.name);
        PrintExprList(e -> data.FUNC.args);
        printf(")");
        break;
    }
}

void PrintExprList(ExprList * es) {
    if (es == nullptr) {
        return;
    }
    printf(",");
    PrintExpr(es -> data);
    PrintExprList(es -> next);
}

void PrintType(Type * t)
{
    int n = t -> ptr_index;
    switch (t -> type)
    {
        case T_VOID:
        {
            printf("void");
            for (int i = 1; i <= n; i++)
                printf("*");
            break;
        }
        case T_INT:
        {
            printf("int");
            for (int i = 1; i <= n; i++)
                printf("*");
            break;
        }
        case T_FUNCPTR:
        {
            PrintType(t -> data.FUNCPTR.ret_type);
            printf("(");
            for (int i = 1; i <= n; i++)
                printf("*");
            printf(")(");
            PrintTypeList(t -> data.FUNCPTR.list);
            printf(")");
            break;
        }
    }
}

void PrintTypeList(TypeList * l)
{
    if (l == nullptr) { return; }
    PrintType(l -> data);
    if (l -> next != nullptr)
    {
        printf(", ");
        PrintTypeList(l -> next);
    }
}

void PrintCmd(Cmd * c) {
    switch (c -> c_type) {
    case T_DECL:
        printf("DECL(");
        PrintVar(c -> data.DECL.var);
        printf(")");
        break;
    case T_ASGN:
        printf("ASGN(");
        PrintExpr(c -> data.ASGN.left);
        printf(",");
        PrintExpr(c -> data.ASGN.right);
        printf(")");
        break;
    case T_SEQ:
        printf("SEQ(");
        PrintCmd(c -> data.SEQ.left);
        printf(",");
        PrintCmd(c -> data.SEQ.right);
        printf(")");
        break;
    case T_IF:
        printf("IF(");
        PrintExpr(c -> data.IF.cond);
        printf(",");
        PrintCmd(c -> data.IF.left);
        printf(",");
        PrintCmd(c -> data.IF.right);
        printf(")");
        break;
    case T_WHILE:
        printf("WHILE(");
        PrintExpr(c -> data.WHILE.cond);
        printf(",");
        PrintCmd(c -> data.WHILE.body);
        printf(")");
        break;
    case T_FOR:
        printf("FOR(");
        PrintCmd(c -> data.FOR.init);
        printf(",");
        PrintExpr(c -> data.FOR.cond);
        printf(",");
        PrintCmd(c -> data.FOR.incr);
        printf(",");
        PrintCmd(c -> data.FOR.body);
        printf(")");
        break;
    case T_DO_WHILE:
        printf("DO_WHILE(");
        PrintCmd(c -> data.DO_WHILE.body);
        printf(",");
        PrintExpr(c -> data.DO_WHILE.cond);
        printf(")");
        break;
    case T_PROC:
        printf("PROC(%s", c -> data.PROC.name);
        PrintExprList(c -> data.PROC.args);
        printf(")");
        break;
    case T_WC:
        printf("WRITE_CHAR(");
        PrintExpr(c -> data.WC.expr);
        printf(")");
        break;
    case T_WI:
        printf("WRITE_INT(");
        PrintExpr(c -> data.WC.expr);
        printf(")");
        break;
    case T_BREAK:
        printf("BREAK");
        break;
    case T_CONTINUE:
        printf("CONTINUE");
        break;
    case T_RETURN:
        printf("RETURN");
        break;
    }
}

void PrintVar(Var * v)
{
    PrintType(v -> type);
    printf(" %s", v -> name);
}

void PrintVarList(VarList * vs) {
    if (vs == nullptr) {
        return;
    }
    PrintVar(vs -> var);
    if (vs -> next != nullptr)
    {
        printf(", ");
        PrintVarList(vs -> next);
    }
}

void PrintGlobItem(GlobItem * g) {
    switch (g -> type) {
    case T_FUNC_DEF:
        printf("FUNC ");
        PrintType(g -> data.FUNC_DEF.ret_type);
        printf(" %s (", g -> data.FUNC_DEF.name);
        PrintVarList(g -> data.FUNC_DEF.args);
        printf(")\n  ");
        PrintCmd(g -> data.FUNC_DEF.body);
        printf("\n\n");
        return;
    case T_GLOB_VAR:
        PrintVar(g -> data.GLOB_VAR.var);
        printf("\n\n");
        return;
    }
}

void PrintGlobItemList(GlobItemList * gs) {
    if (gs == nullptr) {
        return;
    }
    PrintGlobItem(gs -> data);
    PrintGlobItemList(gs -> next);
}