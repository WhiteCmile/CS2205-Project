#include <cstdlib>
#include <vector>
#include "RE.hpp"
#include "lang.hpp"
#include "interpreter.hpp"
#include "basictype.hpp"
#include "value.hpp"
#include "symboltable.hpp"
typedef SymbolTable Table;
typedef unsigned long long Address;
using std :: string;
using std :: shared_ptr;
using std :: vector;

struct ContList 
{
    Cmd * data;
    ContList * next;
};

struct ResProg 
{
    Cmd * foc;
    ContList * ectx;
};

BasicTypePtr TypeProcess(Type * ast_type)
{
/*
    @ret(class of BasicTypePtr) : the real type that comes from Type in AST
    this function is used to turn a Type into BasicTypePtr
*/
    switch(ast_type -> type)
    {
        case T_INT:
            return BasicTypePtr(new TypeInt(ast_type -> ptr_index));
        case T_VOID:
            return BasicTypePtr(new TypeVoid(ast_type -> ptr_index));
        case T_FUNCPTR:
        {
            BasicTypePtr ret_type = TypeProcess(ast_type -> data.FUNCPTR.ret_type);
            vector<BasicTypePtr> vec;
            TypeList * cur = ast_type -> data.FUNCPTR.list;
            while (cur)
            {
                vec.push_back(TypeProcess(cur -> data));
                cur = cur -> next;
            }
            return BasicTypePtr(new TypeFuncPtr(ast_type -> ptr_index, ret_type, vec));
        }
    }
}

void VarDefinition(Var * var, Table * table)
{
/*
    this function is used to process all var definition, it will add this var into the 
    symbol table
*/
    BasicTypePtr type = TypeProcess(var -> type);
    if (type -> GetTypeName() == INT)
        table -> InsertItem(string(var -> name), ValuePtr(new Int(type, ValuePtr(nullptr), 0)), 0);
    else
        table -> InsertItem(string(var -> name), ValuePtr(new FuncPtr(type, ValuePtr(nullptr), nullptr)), 0);
}

TypeList * GetTypeLsFromVarLs(VarList * var_ls)
{
/*
    return a TypeList from a VarList
*/
    if (!var_ls) return nullptr;
    TypeList * cur = new TypeList;
    cur -> data = var_ls -> var -> type;
    cur -> next = GetTypeLsFromVarLs(var_ls -> next);
    return cur;
}

void FuncDefinition(GlobItem * g, Table * table)
{
/*
    this function is used to process function definition.
    our implementation doesn't distinguish between function and procedure
*/
    Type * ret_type = g -> data.FUNC_DEF.ret_type;
    string func_name = string(g -> data.FUNC_DEF.name);
    VarList * arg_list = g -> data.FUNC_DEF.args;
    TypeList * type_list = GetTypeLsFromVarLs(arg_list);
    Type * func_type = TTypeFuncPtr(0, ret_type, type_list);
    BasicTypePtr type = TypeProcess(func_type);
    table -> InsertItem(func_name, ValuePtr(new FuncPtr(type, ValuePtr(nullptr), g)), 1);
    delete func_type;
}

void PreProcess(GlobItemList * c, Table * table)
{
/*
    before the program is interpreted, all global items need to be processed
*/
    GlobItem * cur;
    while (c)
    {
        cur = c -> data;
        if (cur -> type == T_GLOB_VAR)
            VarDefinition(cur-> data.GLOB_VAR.var, table);
        else
            FuncDefinition(cur, table);
        c = c -> next;
    }
    table -> NewFrame(0);
}

ResProg * NewResProgPtr() {
    ResProg * res = new ResProg;
    if (res == nullptr) {
        printf("Failure in malloc.\n");
        exit(0);
    }
    return res;
}

ResProg * InitResProg(Cmd * c) {
    ResProg * res = NewResProgPtr();
    res -> foc = c;
    res -> ectx = nullptr;
    return res;
}

ValuePtr FunctionCall(Expr * e, Table * table, bool need_return = 1)
{
/*
    this function is used to make function call (including procedure call).

*/
    // get expressions of the function and arguments
    Expr * func_expr = e -> data.FUNC.func; 
    ExprList * args_expr = e -> data.FUNC.args; 
    // eval expression of the function
    ValuePtr func = Eval(func_expr, table);
    vector<ValuePtr> arg_values;
    // eval expression of arguments
    while (args_expr)
    {
        arg_values.push_back(Eval(args_expr -> data, table));
        args_expr = args_expr -> next;
    } 
    // get the type of this function
    TypeFuncPtr * func_type = dynamic_cast<TypeFuncPtr*>((func -> GetType()).get());
    // func_type need to be a FuncPtr with its dimension = 0
    if (func_type -> GetTypeName() != FUNCPTR || func_type -> Dim())
        throw RuntimeError("try to apply a non-procedure");
    // check the types of arguments and parameters in order
    GlobItem * call_func = dynamic_cast<FuncPtr*>(func.get()) -> GetFunc();
    vector<BasicTypePtr> para_types = func_type -> GetArgTypes();
    BasicTypePtr ret_type_ptr = func_type -> GetRetType();
    if (para_types.size() != arg_values.size())
        throw RuntimeError("wrong number of arguments");
    for (auto i = 0; i < para_types.size(); i++)
        if (*(arg_values[i] -> GetType()) != *para_types[i])
            throw RuntimeError("argument type differs from parameter type");
    // create frame of the function
    table -> NewFrame(1);
    // define __return
    switch (ret_type_ptr -> GetTypeName())
    {
        case INT:
        {
            table -> InsertItem("__return", ValuePtr(new Int(ret_type_ptr, ValuePtr(nullptr), 0)), 0);
            break;
        }
        case FUNCPTR:
        {
            table -> InsertItem("__return", ValuePtr(new FuncPtr(ret_type_ptr, ValuePtr(nullptr), nullptr)), 0);
            break;
        }
    }
    // bind parameters
    VarList * args = call_func -> data.FUNC_DEF.args;
    for (auto item : arg_values)
    {
        VarDefinition(args -> var, table);
        table -> ModifyByName(args -> var -> name, item);
        args = args -> next;
    }
    // execute function body
    ResProg * cur_func = InitResProg(call_func -> data.FUNC_DEF.body);
    while (!TestEnd(cur_func)) Step(cur_func, table);
    // get the return value
    ValuePtr ret_value = ValuePtr(nullptr);
    if (need_return)
        ret_value = table -> GetValue("__return");
    // delete frame
    table -> DeleteFrame();
    return ret_value;
}

ValuePtr Eval(Expr * e, Table * table, bool required)
{
    switch (e -> e_type)
    {
        case T_CONST:
            return ValuePtr(new Int(e -> data.CONST.val));
        case T_VAR:
            return table -> GetValue(string(e -> data.VAR.name), required);
        case T_BINOP:
        {
            Expr * left = e -> data.BINOP.left, * right = e -> data.BINOP.right;
            switch (e -> data.BINOP.op)
            {
                case T_PLUS:
                    return *(Eval(left, table)) + *(Eval(right, table));
                case T_MINUS:
                    return *(Eval(left, table)) - *(Eval(right, table));
                case T_MUL:
                    return *(Eval(left, table)) * *(Eval(right, table));
                case T_DIV:
                    return *(Eval(left, table)) / *(Eval(right, table));
                case T_MOD:
                    return *(Eval(left, table)) % *(Eval(right, table));
                case T_LT:
                    return *(Eval(left, table)) < *(Eval(right, table));
                case T_GT:
                    return *(Eval(left, table)) > *(Eval(right, table));
                case T_LE:
                    return *(Eval(left, table)) <= *(Eval(right, table));
                case T_GE:
                    return *(Eval(left, table)) >= *(Eval(right, table));
                case T_EQ:
                    return *(Eval(left, table)) == *(Eval(right, table));
                case T_NE:
                    return *(Eval(left, table)) != *(Eval(right, table));
                case T_AND:
                {
                    ValuePtr lval = Eval(left, table);
                    if (!lval) return ValuePtr(new Bool(0));
                    ValuePtr rval = Eval(left, table);
                    return ValuePtr(new Bool((bool) rval));
                }
                case T_OR:
                {
                    ValuePtr lval = Eval(left, table);
                    if (lval) return ValuePtr(new Bool(1));
                    ValuePtr rval = Eval(left, table);
                    return ValuePtr(new Bool((bool) rval));
                }
            }
        }
        case T_UNOP:
        {
            Expr * arg = e -> data.UNOP.arg;
            switch (e -> data.UNOP.op)
            {
                case T_UMINUS:
                    return -(*(Eval(arg, table)));
                case T_NOT:
                    return !(*(Eval(arg, table)));
            }
        }
        case T_DEREF:
        {
            ValuePtr val_ptr = Eval(e -> data.DEREF.arg, table);
            return val_ptr -> GetVal();
        }
        case T_ADDR_OF:
        {
            ValuePtr val = Eval(e -> data.ADDR_OF.arg, table, 0);
            table -> CheckAddress(val.GetAddress());
            return val.MakeAddrOf();
        }
        case T_RI:
        {
            long long val;
            scanf("%lld", &val);
            return ValuePtr(new Int(val));
        }
        case T_FUNC:
            return FunctionCall(e, table);
    }
}

ContList * KSeqCons(Cmd * data, ContList * list)
{
    ContList * new_list = new ContList;
    new_list -> data = data, new_list -> next = list;
    return new_list;
}

ContList * KWhileCons(Cmd * data, ContList * list)
{
    ContList * new_list = new ContList;
    new_list -> data = data, new_list -> next = list;
    return new_list;
}

void Step(ResProg * r, Table * table)
{
    if (r -> foc == nullptr)
    {
        ContList * cl = r -> ectx;
        r -> foc = cl -> data, r -> ectx = cl -> next;
        delete cl;
    }
    else
    {
        Cmd * cmd = r -> foc;
        switch (cmd -> c_type)
        {
            case T_DECL:
            {
                Var * var = cmd -> data.DECL.var;
                VarDefinition(var, table);
                r -> foc = nullptr;
                break;
            }
            case T_NEW_FRAME:
            {
                table -> NewFrame(0);
                r -> foc = nullptr;
                break;
            }
            case T_DEL_FRAME:
            {
                table -> DeleteFrame();
                r -> foc = nullptr;
                break;
            }
            case T_ASGN:
            {
                Expr * left = cmd -> data.ASGN.left, * right = cmd -> data.ASGN.right;
                ValuePtr rval = Eval(cmd -> data.ASGN.right, table);
                switch (left -> e_type)
                {
                    case T_VAR:
                    {
                        table -> ModifyByName(string(left -> data.VAR.name), rval);
                        break;
                    }
                    case T_DEREF:
                    {
                        ValuePtr lval = Eval(cmd -> data.ASGN.left, table);
                        if (*(lval -> GetType()) != *(rval -> GetType()))
                            throw RuntimeError("try to bind a variable to another value whose type is different");
                        table -> CheckAddress(lval.GetAddress());
                        table -> ModifyByAddress(lval.GetAddress(), rval);
                        break;
                    }
                }
                r -> foc = nullptr;
                break;
            }
            case T_SEQ:
            {
                r -> foc = cmd -> data.SEQ.left;
                r -> ectx = KSeqCons(cmd -> data.SEQ.right, r -> ectx);
                break;
            }
            case T_IF:
            {
                if (Eval(cmd -> data.IF.cond, table)) 
                    r -> foc = cmd -> data.IF.left;
                else r -> foc = cmd -> data.IF.right;
                break;
            }
            case T_WHILE:
            {
                if (Eval(cmd -> data.WHILE.cond, table))
                {
                    r -> foc = cmd -> data.WHILE.body;
                    r -> ectx = KWhileCons(cmd, r -> ectx);
                }
                else r -> foc = nullptr;
                break;
            }
            case T_FOR:
            {
            }
            case T_DO_WHILE:
            {
                Cmd * new_cmd = TWhile(cmd -> data.WHILE.cond, cmd -> data.WHILE.body);
                r -> foc = cmd -> data.WHILE.body, r -> ectx = KWhileCons(new_cmd, r -> ectx);
                break;
            }
            case T_PROC:
            {
                Expr * func_call = TFunc(cmd -> data.PROC.func, cmd -> data.PROC.args);
                FunctionCall(func_call, table, 0);
                delete func_call;
                r -> foc = nullptr;
                break;
            }
            case T_WC:
            {
                ValuePtr val_ptr = Eval(cmd -> data.WC.expr, table);
                BasicTypePtr type = val_ptr -> GetType();
                if (type -> GetTypeName() != INT || type -> Dim())
                    throw RuntimeError("try to apply a non-int to write_char");
                auto val = dynamic_cast<Int*>(val_ptr.get());
                printf("%c", val -> GetNum());
                r -> foc = nullptr;
                break;
            }
            case T_WI:
            {
                ValuePtr val_ptr = Eval(cmd -> data.WC.expr, table);
                BasicTypePtr type = val_ptr -> GetType();
                if (type -> GetTypeName() != INT || type -> Dim())
                    throw RuntimeError("try to apply a non-int to write_int");
                auto val = dynamic_cast<Int*>(val_ptr.get());
                printf("%lld", val -> GetNum());
                r -> foc = nullptr;
                break;
            }
            case T_BREAK:
            {
            }
            case T_CONTINUE:
            {
            }
            case T_RETURN:
            {
                r -> foc = nullptr, r -> ectx = nullptr;
                break;
            }
        }
    }
}

int TestEnd(ResProg * r) 
{
    if (r -> foc == nullptr && r -> ectx == nullptr) return 1;
    else return 0;
}