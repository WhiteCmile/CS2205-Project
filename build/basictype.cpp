#include <exception>
#include <vector>
#include "lang.hpp"
#include "basictype.hpp"
using std :: vector;
using std :: exception;

// BasicTypePtr
BasicTypePtr :: BasicTypePtr(BasicType * ptr) : ptr(ptr) {}

BasicType * BasicTypePtr :: operator -> () const { return ptr.get(); }

BasicType & BasicTypePtr :: operator * () const { return *ptr; }

BasicTypePtr :: operator bool() { return ptr.get(); }

// BasicType

BasicType :: BasicType(TypeName type_name, int num) : type_name(type_name), ptr_num(num) {}

bool BasicType :: operator != (const BasicType &op) { return !(*this == op); }

bool BasicType :: operator == (const BasicType &op) { throw exception(); }

TypeName BasicType :: GetTypeName() const { return type_name; }

int BasicType :: GetPtrNum() const { return ptr_num; }

int BasicType :: Dim() const { return ptr_num; }

BasicTypePtr BasicType :: MakePointer() { throw exception(); }

// TypeInt

TypeInt :: TypeInt(int num) : BasicType(INT, num) {}

bool TypeInt :: operator == (const BasicType &op)
{
    if (type_name != op.GetTypeName()) return 0;
    return ptr_num == op.GetPtrNum();
}

BasicTypePtr TypeInt :: MakePointer() 
{ 
    return BasicTypePtr(new TypeInt(ptr_num + 1));
}

// TypeVoid

TypeVoid :: TypeVoid(int num) : BasicType(VOID, num)
{
    if (num) throw exception();
}

bool TypeVoid :: operator == (const BasicType &op) 
{ 
    if (type_name != op.GetTypeName()) return 0;
    return 1; 
}

// TypeFuncPtr

TypeFuncPtr :: TypeFuncPtr(int num, BasicTypePtr ret_type, const vector<BasicTypePtr> &vec)
    : BasicType(FUNCPTR, num), ret_type(ret_type), arg_types(vec) {}

BasicTypePtr TypeFuncPtr :: GetRetType() { return ret_type; }
vector<BasicTypePtr>& TypeFuncPtr :: GetArgTypes() { return arg_types; }

bool TypeFuncPtr :: operator == (const BasicType &op)
{
    if (this == &op) return 1;
    if (type_name != op.GetTypeName()) return 0;
    auto other = dynamic_cast<const TypeFuncPtr&>(op);
    if (ptr_num != other.GetPtrNum()) return 0;
    if (*ret_type != *(other.ret_type)) return 0;
    size_t n1 = arg_types.size(), n2 = other.arg_types.size();
    if (n1 != n2) return 0;
    for (auto i = 0; i < n1; i++)
        if (*(arg_types[i]) != *(other.arg_types[i])) return 0;
    return 1;
}

BasicTypePtr TypeFuncPtr :: MakePointer()
{
    return BasicTypePtr(new TypeFuncPtr(ptr_num + 1, ret_type, arg_types));
}