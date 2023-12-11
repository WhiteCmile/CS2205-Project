#include <cstdarg>
#include <cstdint>
#include <vector>
#include "RE.hpp"
#include "value.hpp"
#include "basictype.hpp"

// ValuePtr
ValuePtr :: ValuePtr(BasicValue * ptr) : ptr(ptr) {}

ValuePtr ValuePtr :: MakeAddrOf()
{
    BasicTypePtr type_ptr = (*this) -> GetType();
    TypeName type_name = type_ptr -> GetTypeName();
    switch (type_name)
    {
        case INT:
            return ValuePtr(new Int(type_ptr -> MakePointer(), *this, 0));
        case FUNCPTR:
            return ValuePtr(new FuncPtr(type_ptr -> MakePointer(), *this, nullptr));
    }
}

BasicValue * ValuePtr :: operator -> () { return ptr.get(); }

BasicValue & ValuePtr :: operator * () { return *ptr; }

ValuePtr :: operator bool() { return *(ptr.get()); }

unsigned long long ValuePtr :: GetAddress() const
{ 
    return reinterpret_cast<std :: uintptr_t>(ptr.get());
}

bool ValuePtr :: operator == (const ValuePtr &other)
{
    return this -> GetAddress() == other.GetAddress();
}

BasicValue * ValuePtr :: get() { return ptr.get(); }

// BasicValue

BasicValue :: BasicValue(BasicTypePtr type, ValuePtr ptr)
    : type(type), val(ptr) {}

BasicTypePtr BasicValue :: GetType() const { return type; }
ValuePtr BasicValue :: GetVal() const { return val; }
void BasicValue :: Modify(ValuePtr new_val) { val = new_val; }

ValuePtr BasicValue :: operator + (const BasicValue &op) { throw RuntimeError("try to apply a binary operator to two operands with at least one of it being not int"); }
ValuePtr BasicValue :: operator - (const BasicValue &op) { throw RuntimeError("try to apply a binary operator to two operands with at least one of it being not int"); }
ValuePtr BasicValue :: operator - () { throw RuntimeError("try to apply a binary operator to two operands with at least one of it being not int"); }
ValuePtr BasicValue :: operator * (const BasicValue &op) { throw RuntimeError("try to apply a binary operator to two operands with at least one of it being not int"); }
ValuePtr BasicValue :: operator / (const BasicValue &op) { throw RuntimeError("try to apply a binary operator to two operands with at least one of it being not int"); }
ValuePtr BasicValue :: operator % (const BasicValue &op) { throw RuntimeError("try to apply a binary operator to two operands with at least one of it being not int"); }

ValuePtr BasicValue :: operator < (const BasicValue &op) { throw RuntimeError("try to apply a binary operator to two operands with at least one of it being not int"); }
ValuePtr BasicValue :: operator <= (const BasicValue &op) { throw RuntimeError("try to apply a binary operator to two operands with at least one of it being not int"); }
ValuePtr BasicValue :: operator == (const BasicValue &op) { throw RuntimeError("try to apply a binary operator to two operands with at least one of it being not int"); }
ValuePtr BasicValue :: operator != (const BasicValue &op) { return ValuePtr(new Bool(!(*this == op))); }
ValuePtr BasicValue :: operator >= (const BasicValue &op) { throw RuntimeError("try to apply a binary operator to two operands with at least one of it being not int"); }
ValuePtr BasicValue :: operator > (const BasicValue &op) { throw RuntimeError("try to apply a binary operator to two operands with at least one of it being not int"); }

BasicValue :: operator bool() { throw RuntimeError("undefined error"); }

ValuePtr BasicValue :: operator ! () { return ValuePtr(new Bool(!(bool) (*this))); }

ValuePtr & BasicValue :: operator * () { throw RuntimeError("undefined error"); }

// Bool

Bool :: Bool(BasicTypePtr type, ValuePtr ptr, bool b) : BasicValue(type, ptr), flag(b) {}

Bool :: Bool(bool b) : BasicValue(BasicTypePtr(new TypeBool(0)), ValuePtr(nullptr)), flag(b) {}

Bool :: operator bool() { return GetBool(); }

bool Bool :: GetBool() { return flag; }

// Int

Int :: Int(BasicTypePtr type, ValuePtr ptr, long long m) 
    : BasicValue(type, ptr), n(m) {}

Int :: Int(long long m) : BasicValue(BasicTypePtr(new TypeInt(0)), ValuePtr(nullptr)), n(m) {}

long long Int :: GetNum() { return n; }

void Int :: ChangeNum(long long m) { n = m; }

// check if all the operands are int
void CheckOperand(const BasicValue &op1)
{
    BasicTypePtr type = op1.GetType();
    if (type -> Dim() || type -> GetTypeName() != INT) 
        throw RuntimeError("try to apply a binary operator to two operands with at least one of it being not int");
}
void CheckOperands(const BasicValue &op1, const BasicValue &op2)
{
    CheckOperand(op1), CheckOperand(op2);
}

ValuePtr Int :: operator + (const BasicValue &op)
{
    CheckOperands(*this, op);
    auto other = dynamic_cast<const Int&>(op);
    return ValuePtr(new Int(BasicTypePtr(new TypeInt(0)), ValuePtr(nullptr), n + other.n));
}

ValuePtr Int :: operator - (const BasicValue &op)
{
    CheckOperands(*this, op);
    auto other = dynamic_cast<const Int&>(op);
    return ValuePtr(new Int(BasicTypePtr(new TypeInt(0)), ValuePtr(nullptr), n - other.n));
}

ValuePtr Int :: operator - ()
{
    CheckOperand(*this);
    return ValuePtr(new Int(BasicTypePtr(new TypeInt(0)), ValuePtr(nullptr), -n));
}

ValuePtr Int :: operator * (const BasicValue &op)
{
    CheckOperands(*this, op);
    auto other = dynamic_cast<const Int&>(op);
    return ValuePtr(new Int(BasicTypePtr(new TypeInt(0)), ValuePtr(nullptr), n * other.n));
}

ValuePtr Int :: operator / (const BasicValue &op)
{
    CheckOperands(*this, op);
    auto other = dynamic_cast<const Int&>(op);
    return ValuePtr(new Int(BasicTypePtr(new TypeInt(0)), ValuePtr(nullptr), n / other.n));
}

ValuePtr Int :: operator % (const BasicValue &op)
{
    CheckOperands(*this, op);
    auto other = dynamic_cast<const Int&>(op);
    return ValuePtr(new Int(BasicTypePtr(new TypeInt(0)), ValuePtr(nullptr), n % other.n));
}

ValuePtr Int :: operator < (const BasicValue &op)
{
    CheckOperands(*this, op);
    auto other = dynamic_cast<const Int&>(op);
    return ValuePtr(new Bool(n < other.n));
}

ValuePtr Int :: operator <= (const BasicValue &op)
{
    CheckOperands(*this, op);
    auto other = dynamic_cast<const Int&>(op);
    return ValuePtr(new Bool(n <= other.n));
}

ValuePtr Int :: operator >= (const BasicValue &op)
{
    CheckOperands(*this, op);
    auto other = dynamic_cast<const Int&>(op);
    return ValuePtr(new Bool(n >= other.n));
}

ValuePtr Int :: operator > (const BasicValue &op)
{
    CheckOperands(*this, op);
    auto other = dynamic_cast<const Int&>(op);
    return ValuePtr(new Bool(n > other.n));
}

ValuePtr Int :: operator == (const BasicValue &op)
{
    if (type != op.GetType()) return 0;
    auto other = dynamic_cast<const Int &>(op);
    if (!type -> Dim()) return ValuePtr(new Bool(n == other.n));
    if (!val || !other.GetVal()) return 0;
    return ValuePtr(new Bool(val == other.GetVal()));
}

Int :: operator bool()
{
    if (!type -> Dim()) return n;
    return 1;
}

ValuePtr & Int :: operator * ()
{
    if (!val) throw RuntimeError("undefined pointer");
    return val;
}

// FuncPtr

FuncPtr :: FuncPtr(BasicTypePtr type, ValuePtr ptr, GlobItem * name)
    : BasicValue(type, ptr), func(name) {}

GlobItem * FuncPtr :: GetFunc() { return func; }

void FuncPtr :: ChangeGlobItem(GlobItem * gi) { func = gi; }

FuncPtr :: operator bool()
{
    return 1;
}

ValuePtr FuncPtr :: operator == (const BasicValue &op)
{
    if (type != op.GetType()) return 0;
    if (!val || !op.GetVal()) return 0;
    return ValuePtr(new Bool(val == op.GetVal()));
}

ValuePtr & FuncPtr :: operator * ()
{
    return val;
}