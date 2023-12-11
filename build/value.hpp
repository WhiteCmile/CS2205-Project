#ifndef VALUE_HPP
#define VALUE_HPP

/*
    When a Int is defined but not initialized, it is 0 by default.
    When a pointer is defined but not initialized, *f is invalid.
*/

#include <memory>
#include "lang.hpp"
#include "basictype.hpp"
#include "value.hpp"
using std :: shared_ptr;

class BasicValue;

class ValuePtr;

class ValuePtr
{
private:
/*
    @attr ptr(shared pointer of class BasicValue): reduce memory leak
*/
    shared_ptr<BasicValue> ptr;
public:
    // constructor
    ValuePtr(BasicValue *);

    BasicValue * operator -> (); // operator -> 
    BasicValue & operator * (); // operator *
    operator bool();
    unsigned long long GetAddress() const;
    bool operator == (const ValuePtr &);

    BasicValue * get();
};

class BasicValue
{
protected:
/*
    @attr type(class BasicTypePtr): a pointer to the type of this value
    @attr val(class ValuePtr): if this object is a pointer, then val stores which object 
        it points to; otherwise it is null
*/
    BasicTypePtr type;
    ValuePtr val;
public:
    // constructor
    BasicValue(BasicTypePtr type, ValuePtr val); 

    BasicTypePtr GetType() const;
    ValuePtr GetVal() const;

    virtual ValuePtr operator + (const BasicValue &op); // E + E
    virtual ValuePtr operator - (const BasicValue &op); // E - E
    virtual ValuePtr operator - (); // - E
    virtual ValuePtr operator * (const BasicValue &op); // E * E
    virtual ValuePtr operator / (const BasicValue &op); // E / E
    virtual ValuePtr operator % (const BasicValue &op); // E % E

    virtual ValuePtr operator < (const BasicValue &op); // E < E
    virtual ValuePtr operator <= (const BasicValue &op); // E <= E
    virtual ValuePtr operator == (const BasicValue &op); // E == E
    virtual ValuePtr operator != (const BasicValue &op); // E != E
    virtual ValuePtr operator >= (const BasicValue &op); // E >= E
    virtual ValuePtr operator > (const BasicValue &op); // E > E

    virtual operator bool();
    virtual ValuePtr operator ! (); // !E

    virtual ValuePtr operator & (); // & E
    virtual ValuePtr & operator * (); // * E
};

class Bool : public BasicValue
{
private:
/*
    @attr flag(bool): bool value in C++ of a Bool object
*/
    bool flag;
public:
    Bool(BasicTypePtr type, ValuePtr ptr, bool b);
    Bool(bool);

    operator bool();

    bool GetBool();
};

class Int : public BasicValue
{
private:
/*
    @attr n(long long): if dimension of an Int is 0, then v stores its value; otherwise v = 0
*/
    long long n;
public:
    // constructor
    Int(BasicTypePtr type, ValuePtr ptr, long long m);
    Int(long long);

    long long GetNum();

    ValuePtr operator + (const BasicValue &op) override; // a + b
    ValuePtr operator - (const BasicValue &op) override; // a - b
    ValuePtr operator - () override; // -b
    ValuePtr operator * (const BasicValue &op) override; // a * b
    ValuePtr operator / (const BasicValue &op) override; // a / b
    ValuePtr operator % (const BasicValue &op) override; // a % b

    ValuePtr operator < (const BasicValue &op) override; // a < b
    ValuePtr operator <= (const BasicValue &op) override; // a <= b
    ValuePtr operator == (const BasicValue &op) override; // a == b
    ValuePtr operator >= (const BasicValue &op) override; // a >= b
    ValuePtr operator > (const BasicValue &op) override; // a > b

    operator bool() override;

    ValuePtr operator & () override;
    ValuePtr & operator * () override;
};

class FuncPtr : public BasicValue
{
private:
/*
    @attr func_name(pointer of GlobItem): if dimension of an FuncPtr is 0, then 
    func_name stores the function in AST
*/
    GlobItem * func;
public:
    // constructor
    FuncPtr(BasicTypePtr type, ValuePtr ptr, GlobItem * name);

    GlobItem * GetFunc();
    operator bool() override;

    ValuePtr operator == (const BasicValue &op) override; // FuncPtr == FuncPtr

    ValuePtr operator & () override; // & E
    ValuePtr & operator * () override; // * E
};

#endif