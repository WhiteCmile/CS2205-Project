#ifndef VALUE_HPP
#define VALUE_HPP

/*
    When a Int is defined but not initialized, it is 0 by default.
    When a pointer is defined but not initialized, *f is invalid.
*/

#include <memory>
#include "lang.hpp"
#include "basictype.hpp"
using std :: shared_ptr;

class BasicValue;

class ValuePtr;

class Int :: BasicValue;

class FuncPtr :: BasicValue;

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
    long long get();
};

class BasicValue
{
protected:
/*
    @attr type(class BasicTypePtr): type of this value
    @attr val(class ValuePtr): if this object is a pointer, then val stores which object 
        it points to; otherwise it is null
*/
    BasicTypePtr type;
    ValuePtr val;
public:
    // constructor
    BasicValue(BasicTypePtr type, ValuePtr val); 

    virtual ValuePtr operator + (const BasicValue &op); // E + E
    virtual ValuePtr operator - (const BasicValue &op); // E - E
    virtual ValuePtr operator - (); // - E
    virtual ValuePtr operator * (const BasicValue &op); // E * E
    virtual ValuePtr operator / (const BasicValue &op); // E / E
    virtual ValuePtr operator % (const BasicValue &op); // E % E

    virtual bool operator < (const BasicValue &op); // E < E
    virtual bool operator <= (const BasicValue &op); // E <= E
    virtual bool operator == (const BasicValue &op); // E == E
    virtual bool operator != (const BasicValue &op); // E != E
    virtual bool operator >= (const BasicValue &op); // E >= E
    virtual bool operator > (const BasicValue &op); // E > E

    virtual operator bool() = 0;
    virtual bool operator ! (); // !E
    virtual bool operator && (const BasicValue &op); // E && E
    virtual bool operator || (const BasicValue &op); // E || E

    virtual ValuePtr operator & () = 0; // & E
    virtual ValuePtr & operator * () = 0; // * E
};

class Int :: BasicValue
{
private:
/*
    @attr n(long long): if dimension of an Int is 0, then v stores its value; otherwise v = 0
*/
    long long n;
public:
    // constructor
    Int(BasicType type, ValuePtr ptr, long long m);

    ValuePtr operator + (const Int &op) override; // a + b
    ValuePtr operator - (const Int &op) override; // a - b
    ValuePtr operator - () override; // -b
    ValuePtr operator * (const Int &op) override; // a * b
    ValuePtr operator / (const Int &op) override; // a / b
    ValuePtr operator % (const Int &op) override; // a % b

    bool operator < (const Int &op) override; // a < b
    bool operator <= (const Int &op) override; // a <= b
    bool operator == (const Int &op) override; // a == b
    bool operator != (const Int &op) override; // a != b
    bool operator >= (const Int &op) override; // a >= b
    bool operator > (const Int &op) override; // a > b

    operator bool() override;

    ValuePtr operator & () override;
    ValuePtr & operator * () override;
};

class FuncPtr :: BasicValue
{
public:
    // constructor
    FuncPtr(BasicType type, ValuePtr ptr);

    operator bool() override;

    bool operator == (const FuncPtr &op) override; // FuncPtr == FuncPtr
    bool operator != (const FuncPtr &op) override; // FuncPtr == FuncPtr

    ValuePtr operator & () override; // & E
    ValuePtr & operator * () override; // * E
};

#endif