#ifndef BASICTYPE_HPP
#define BASICTYPE_HPP

#include "lang.hpp"
#include <vector>
#include <memory>
using std :: vector;
using std :: shared_ptr;

enum TypeName
{
    INT, VOID, FUNCPTR
};

class BasicType;

class BasicTypePtr;

class TypeInt :: BasicType;

class TypeVoid :: BasicType;

class TypeFuncPtr :: BasicType;

class BasicTypePtr
{
private:
/*
    @attr ptr(shared pointer of class BasicType): reduce memory leak
*/
    shared_ptr<BasicType> ptr;
public:
    // constructor
    BasicTypePtr(BasicType *);

    BasicType * operator -> (); // operator -> 
    BasicType & operator * (); // operator *
    operator bool();
};

class BasicType
{
protected:
/*
    @attr type_name(enum TypeName): indicating which type this object is
    @attr ptr_num(int): dimension of the pointer(int -> 0, int * -> 1, int ** -> 2)
*/
    TypeName type_name;
    int ptr_num;
public:
    // constructor
    BasicType(TypeName type_name, int num);

    virtual bool operator == (const BasicType &op); // E == E
    virtual bool operator != (const BasicType &op); // E != E
};

class TypeInt :: BasicType
{
public:
    // constructor
    TypeInt(int num);

    bool IsInt(); // check whether this type is a pointer

    bool operator == (const TypeInt &op) override; // a == b
    TypeInt MakePointer();
};

class TypeVoid :: BasicType
{
public:
    //constructor
    TypeVoid(int num);

    bool operator == (const TypeVoid &op) override; // TypeVoid == TypeVoid
};

class TypeFuncPtr :: BasicType
{
private:
/*
    @attr ret_type(class BasicTypePtr): return type of this function pointer
    @attr arg_types(vector of class BasciTypePtr): type list of arguments
*/
    BasicTypePtr ret_type;
    vector<BasicTypePtr> arg_types;
public:
    // constructor
    TypeFuncPtr(int num, BasicTypePtr ret_type, const vector<BasicTypePtr> &vec, int num, bool flag, BasicTypePtr ptr);

    bool operator == (const TypeFuncPtr &op) override; // TypeFuncPtr == TypeFuncPtr
    TypeFuncPtr MakePointer(); // return a FuncPtr whose dimension is this -> ptr_num + 1
};

#endif