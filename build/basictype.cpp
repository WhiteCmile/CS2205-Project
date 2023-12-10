#include <exception>
#include <vector>
#include "lang.hpp"
#include "basictype.hpp"
using std :: vector;

// TypePtr
TypePtr :: TypePtr(BasicType * ptr) : ptr(ptr) {}

TypePtr :: BasicType * operator -> () { return ptr.get(); }

TypePtr :: BasicType & operator * () { return *ptr; }

TypePtr :: operator bool() { return ptr.get(); }

// BasicType

BasicType :: BasicType(TypeName type_name, int num) : type_name(type_name), ptr_num(num) {}

BasicType :: bool operator == (const BasicType &op) { return 0; }

BasicType :: bool operator != (const BasicType &op) { return !(*this == op); }

// TypeInt

TypeInt :: TypeInt(int num) : type_name(INT), ptr_num(num) {}

bool IsInt() { return !ptr_num; }

TypeInt :: bool operator == (const TypeInt &op) override
{
    return ptr_num == op.ptr_num;
}

TypeInt :: TypeInt MakePointer() { return TypeInt(ptr_num + 1); }

// TypeVoid

TypeVoid :: TypeVoid(int num) : type_name(VOID), ptr_num(num) 
{
    if (num) throw exception();
}

TypeVoid :: bool operator == (const TypeVoid &op) override { return 1; }

// TypeFuncPtr

TypeFuncPtr :: TypeFuncPtr(int num, TypePtr ret_type, const vector<TypePtr> &vec)
    : type_name(FUNCPTR), ptr_num(num), ret_type(ret_type), arg_types(vec) {}

TypeFuncPtr :: bool operator == (const TypeFuncPtr &op) override
{
    if (this == &op) return 1;
    if (ptr_num != op.ptr_num) return 0;
    if (*ret_type != *(op.ret_type)) return 0;
    size_t n1 = arg_types.size(), n2 = op.arg_types.size();
    if (n1 != n2) return 0;
    for (auto i = 0; i < n1; i++)
        if (*(arg_types[i]) != *(op.arg_types[i])) return 0;
    return 1;
}

TypeFuncPtr :: TypeFuncPtr MakePointer()
{
    return TypeFuncPtr(ptr_num + 1, ret_type, arg_types);
}