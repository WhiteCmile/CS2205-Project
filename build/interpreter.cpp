#include <cstdlib>
#include <vector>
#include "lang.hpp"
#include "interpreter.hpp"
#include "basictype.hpp"
#include "value.hpp"
#include "symboltable.hpp"
typedef SymbolTable Table;
using std :: string;
using std :: shared_ptr;
using std :: vector;

struct ContList 
{
};

struct ResProg 
{
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
    table -> NewFrame(0);
    table -> InsertItem("__return", ValuePtr(new Int(BasicTypePtr(new TypeInt(0)), ValuePtr(nullptr), 0)), 0);
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
}

// ResProg * NewResProgPtr() {
//     ResProg * res = new ResProg;
//     if (res == nullptr) {
//         printf("Failure in malloc.\n");
//         exit(0);
//     }
//     return res;
// }

// ResProg * InitResProg(GlobItemList * c) {
//     ResProg * res = NewResProgPtr();
//     res -> foc = c;
//     res -> ectx = CL_Nil();
//     return res;
// }

void step(ResProg * r) 
{
}

int TestEnd(ResProg * r) 
{
    // if (r -> foc == nullptr && r -> ectx == nullptr) return 1;
    // else return 0;
}