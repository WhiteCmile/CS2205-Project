#include <exception>
#include <cstdarg>
#include <cstdint>
#include <vector>
#include "value.hpp"
#include "basictype.hpp"
using std :: exception;

// ValuePtr
ValuePtr :: ValuePtr(BasicValue * ptr) : ptr(ptr) {}

BasicValue * ValuePtr :: operator -> () { return ptr.get(); }

BasicValue & ValuePtr :: operator * () { return *ptr; }

ValuePtr :: operator bool() { return ptr.get(); }

unsigned long long ValuePtr :: GetAddress() const
{ 
    return reinterpret_cast<std :: uintptr_t>(ptr.get());
}

bool ValuePtr :: operator == (const ValuePtr &other)
{
    return this -> GetAddress() == other.GetAddress();
}

// BasicValue

BasicValue :: BasicValue(BasicTypePtr type, ValuePtr ptr)
    : type(type), val(ptr) {}

BasicTypePtr BasicValue :: GetType() const { return type; }
ValuePtr BasicValue :: GetVal() const { return val; }

ValuePtr BasicValue :: operator + (const BasicValue &op) { throw exception(); }
ValuePtr BasicValue :: operator - (const BasicValue &op) { throw exception(); }
ValuePtr BasicValue :: operator - () { throw exception(); }
ValuePtr BasicValue :: operator * (const BasicValue &op) { throw exception(); }
ValuePtr BasicValue :: operator / (const BasicValue &op) { throw exception(); }
ValuePtr BasicValue :: operator % (const BasicValue &op) { throw exception(); }

bool BasicValue :: operator < (const BasicValue &op) { throw exception(); }
bool BasicValue :: operator <= (const BasicValue &op) { throw exception(); }
bool BasicValue :: operator != (const BasicValue &op) { return !(*this == op); }
bool BasicValue :: operator >= (const BasicValue &op) { throw exception(); }
bool BasicValue :: operator > (const BasicValue &op) { throw exception(); }

bool BasicValue :: operator ! () { return !(*this); }

bool BasicValue :: operator && (const BasicValue &op)
{
    return (*this) && op;
}

bool BasicValue :: operator || (const BasicValue &op)
{
    return (*this) || op;
}

// Int

Int :: Int(BasicTypePtr type, ValuePtr ptr, long long m) 
    : BasicValue(type, ptr), n(m) {}

long long Int :: GetNum() { return n; }

// check if all the operands are int
void CheckOperand(const BasicValue &op1)
{
    BasicTypePtr type = op1.GetType();
    if (type -> Dim() || type -> GetTypeName() != INT) 
        throw exception();
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

bool Int :: operator < (const BasicValue &op)
{
    CheckOperands(*this, op);
    auto other = dynamic_cast<const Int&>(op);
    return n < other.n;
}

bool Int :: operator <= (const BasicValue &op)
{
    CheckOperands(*this, op);
    auto other = dynamic_cast<const Int&>(op);
    return n <= other.n;
}

bool Int :: operator >= (const BasicValue &op)
{
    CheckOperands(*this, op);
    auto other = dynamic_cast<const Int&>(op);
    return n >= other.n;
}

bool Int :: operator > (const BasicValue &op)
{
    CheckOperands(*this, op);
    auto other = dynamic_cast<const Int&>(op);
    return n > other.n;
}

bool Int :: operator == (const BasicValue &op)
{
    if (type != op.GetType()) return 0;
    auto other = dynamic_cast<const Int &>(op);
    if (!type -> Dim()) return n == other.n;
    if (!val || !other.GetVal()) return 0;
    return val == other.GetVal();
}

Int :: operator bool()
{
    if (!type -> Dim()) return n;
    return 1;
}

ValuePtr Int :: operator & ()
{
    return ValuePtr(new Int(type -> MakePointer(), ValuePtr(this), 0));
}

ValuePtr & Int :: operator * ()
{
    if (!val) throw exception();
    return val;
}

// FuncPtr

FuncPtr :: FuncPtr(BasicTypePtr type, ValuePtr ptr, GlobItem * name)
    : BasicValue(type, ptr), func(name) {}

GlobItem * FuncPtr :: GetFunc() { return func; }

FuncPtr :: operator bool()
{
    return 1;
}

bool FuncPtr :: operator == (const BasicValue &op)
{
    if (type != op.GetType()) return 0;
    if (!val || !op.GetVal()) return 0;
    return val == op.GetVal();
}

ValuePtr FuncPtr :: operator & ()
{
    return ValuePtr(new FuncPtr(type -> MakePointer(), ValuePtr(this), nullptr));
}

ValuePtr & FuncPtr :: operator * ()
{
    return val;
}