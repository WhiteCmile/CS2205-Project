#ifndef BASICTYPE_HPP
#define BASICTYPE_HPP

#include <vector>
#include <memory>
using std :: vector;
using std :: shared_ptr;

enum TypeName
{
    INT, VOID, FUNCPTR, BOOL
};

class BasicType;

class BasicTypePtr;

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

    BasicType * operator -> () const; // operator -> 
    BasicType & operator * () const; // operator *
    operator bool();

    BasicType * get();
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

    TypeName GetTypeName() const; 
    int GetPtrNum() const;

    int Dim() const; // dimension

    virtual bool operator == (const BasicType &op); // E == E
    virtual bool operator != (const BasicType &op); // E != E
    virtual BasicTypePtr MakePointer();
};

class TypeInt : public BasicType
{
public:
    // constructor
    TypeInt(int num);

    bool operator == (const BasicType &op) override; // a == b
    BasicTypePtr MakePointer() override;
};

class TypeVoid : public BasicType
{
public:
    //constructor
    TypeVoid(int num);

    bool operator == (const BasicType &op) override; // TypeVoid == TypeVoid
};

class TypeBool : public BasicType
{
public:
    //constructor
    TypeBool(int num);

    bool operator == (const BasicType &op) override; // TypeBool == TypeBool
};

class TypeFuncPtr : public BasicType
{
private:
/*
    @attr ret_type(class BasicTypePtr): return type of this function pointer
    @attr arg_types(vector of class BasicTypePtr): type list of arguments
*/
    BasicTypePtr ret_type;
    vector<BasicTypePtr> arg_types;
public:
    // constructor
    TypeFuncPtr(int num, BasicTypePtr ret_type, const vector<BasicTypePtr> &vec);

    BasicTypePtr GetRetType();
    vector<BasicTypePtr>& GetArgTypes();

    bool operator == (const BasicType &op) override; // TypeFuncPtr == TypeFuncPtr
    BasicTypePtr MakePointer() override; // return a FuncPtr whose dimension is this -> ptr_num + 1
};

#endif