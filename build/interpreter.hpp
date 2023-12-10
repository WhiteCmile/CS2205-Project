#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "lang.hpp"
#include "symboltable.hpp"

struct ResProg;
ResProg * InitResProg(GlobItemList * c);
void PreProcess(GlobItemList *, SymbolTable *);
void Step(ResProg * r);
int TestEnd(ResProg * r);

#endif
