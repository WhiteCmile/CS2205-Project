#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "lang.hpp"
#include "symboltable.hpp"
#include "value.hpp"

struct ResProg;
ResProg * InitResProg(Cmd *);
void PreProcess(GlobItemList *, SymbolTable *);
ValuePtr Eval(Expr *, SymbolTable *, bool required = 1);
void Step(ResProg *, SymbolTable *);
int TestEnd(ResProg *);

#endif
