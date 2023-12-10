#include <exception>
#include <cstdarg>
#include <vector>
#include "lang.hpp"
#include "basictype.hpp"

// ValuePtr
ValuePtr :: ValuePtr(BasicValue * ptr) : ptr(ptr) {}

ValuePtr :: BasicValue * operator -> () { return ptr.get(); }

ValuePtr :: BasicValue & operator * () { return *ptr; }

ValuePtr :: operator bool() { return ptr.get(); }

ValuePtr :: long long get() { return ptr.get(); }

// BasicValue

BasicValue :: BasicValue(BasicTypePtr type, ValuePtr ptr)
    : type(type), val(ptr) {}

BasicValue :: ValuePtr operator + (const BasicValue &op) { throw exception(); }
BasicValue :: ValuePtr operator - (const BasicValue &op) { throw exception(); }
BasicValue :: ValuePtr operator - () { throw exception(); }
BasicValue :: ValuePtr operator * (const BasicValue &op) { throw exception(); }
BasicValue :: ValuePtr operator / (const BasicValue &op) { throw exception(); }
BasicValue :: ValuePtr operator % (const BasicValue &op) { throw exception(); }

BasicValue :: bool operator < (const BasicValue &op) { throw exception(); }
BasicValue :: bool operator <= (const BasicValue &op) { throw exception(); }
BasicValue :: bool operator == (const BasicValue &op) { return 0; }
BasicValue :: bool operator != (const BasicValue &op) { return 1; }
BasicValue :: bool operator >= (const BasicValue &op) { throw exception(); }
BasicValue :: bool operator > (const BasicValue &op) { throw exception(); }

BasicValue :: bool operator ! () { return !(this -> bool()); }

BasicValue :: virtual bool operator && (const BasicValue &op)
{
    return this -> bool() && op.bool();
}

BasicValue :: virtual bool operator || (const BasicValue &op)
{
    return this -> bool() | op.bool();
}

// Int

Int :: Int(BasicType type, ValuePtr ptr, long long m) 
    : type(type), val(ptr), n(val) {}

// check if all the operands are int
void CheckOperand(const Int &op1)
{
    if (!op1.type -> IsInt()) throw exception();
}
void CheckOperands(const Int &op1, const Int &op2)
{
    CheckOperand(op1), CheckOperand(op2);
}

Int :: ValuePtr operator + (const Int &op) override
{
    CheckOperands(*this, op);
    return ValuePtr(new Int(TypeInt(0), ValuePtr(nullptr), n + op.n));
}

Int :: ValuePtr operator - (const Int &op) override
{
    CheckOperands(*this, op);
    return ValuePtr(new Int(TypeInt(0), ValuePtr(nullptr), n - op.n));
}

Int :: ValuePtr operator - () override
{
    CheckOperand(*this);
    return ValuePtr(new Int(TypeInt(0), ValuePtr(nullptr), -n));
}

Int :: ValuePtr operator * (const Int &op) override
{
    CheckOperands(*this, op);
    return ValuePtr(new Int(TypeInt(0), ValuePtr(nullptr), n * op.n));
}

Int :: ValuePtr operator / (const Int &op) override
{
    CheckOperands(*this, op);
    return ValuePtr(new Int(TypeInt(0), ValuePtr(nullptr), n / op.n));
}

Int :: ValuePtr operator % (const Int &op) override
{
    CheckOperands(*this, op);
    return ValuePtr(new Int(TypeInt(0), ValuePtr(nullptr), n % op.n));
}

Int :: bool operator < (const Int &op) override
{
    CheckOperands(*this, op);
    return n < op.n;
}

Int :: bool operator <= (const Int &op) override
{
    CheckOperands(*this, op);
    return n <= op.n;
}

Int :: bool operator == (const Int &op) override
{
    if (type != op.type) return 0;
    if (type.IsInt()) return n == op.n;
    if (!val || !op.val) return 0;
    return val == op.val;
}

Int :: bool operator != (const Int &op) override
{
    return !(*this == op);
}

Int :: bool operator >= (const Int &op) override
{
    CheckOperands(*this, op);
    return n >= op.n;
}

Int :: bool operator > (const Int &op) override
{
    CheckOperands(*this, op);
    return n > op.n;
}

Int :: operator bool() override
{
    if (type.IsInt()) return n;
    return 1;
}

Int :: ValuePtr operator & () override
{
    return ValuePtr(new Int(type -> MakePointer(), ValuePtr(this), 0));
}

Int :: ValuePtr& operator * () override
{
    if (!val) throw exception();
    return *val;
}

// FuncPtr

FuncPtr :: FuncPtr(BasicType type, ValuePtr ptr)
    : type(type), val(ptr) {}

FuncPtr :: operator bool() override
{
    return 1;
}

FuncPtr :: bool operator == (const FuncPtr &op) override
{
    if (type != op.type) return 0;
    if (!val || !op.val) return 0;
    return val == op.val;
}

FuncPtr :: bool operator != (const FuncPtr &op) override
{
    return !(*this == op);
}

FuncPtr :: FuncPtr operator & () override
{
    return FuncPtr(type -> MakePointer(), 0, ValuePtr(this), 0);
}

FuncPtr :: FuncPtr & operator * () override
{
    return *val;
}

FuncPtr :: FuncPtr & operator = (const FuncPtr &op) override
{
    if (this == &op) return *this;
    if (type != op.type) throw exception();
    val = op.val;
    return *this;
}