#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "lang.hpp"
#include "symboltable.hpp"

struct ResProg;
ResProg * InitResProg(Cmd *);
void PreProcess(GlobItemList *, SymbolTable *);
void Eval(Expr *);
void Step(ResProg *, SymbolTable *);
int TestEnd(ResProg *);

#endif
