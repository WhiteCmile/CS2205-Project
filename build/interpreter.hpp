#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "lang.hpp"

struct ResProg;
ResProg * InitResProg(GlobItemList * c);
void Step(ResProg * r);
int TestEnd(ResProg * r);

#endif
