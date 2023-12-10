#include <cstdlib>
#include "lang.hpp"
#include "interpreter.hpp"

struct ContList {
  struct cmd * c;
  struct cont_list * link;
};

struct res_prog {
  struct cmd * foc;
  struct cont_list * ectx;
};

struct SLL_hash_table * var_state;

struct res_prog * new_res_prog_ptr() {
  struct res_prog * res = (struct res_prog *) malloc(sizeof(struct res_prog));
  if (res == nullptr) {
    printf("Failure in malloc.\n");
    exit(0);
  }
  return res;
}

struct cont_list * new_cont_list_ptr() {
  struct cont_list * res = (struct cont_list *) malloc(sizeof(struct cont_list));
  if (res == nullptr) {
    printf("Failure in malloc.\n");
    exit(0);
  }
  return res;
}

struct cont_list * CL_Nil() {
  return nullptr;
}

struct cont_list * CL_KSeqCons(struct cmd * c, struct cont_list * l)
{
    struct cont_list * cl = new_cont_list_ptr();
    if (c == nullptr) { cl = l; return cl; }
    switch(c -> t)
    {
        case T_SEQ:
            cl -> c = c;
            cl -> link = l;
            break;
        default:
            cl -> c = TSeq(c, nullptr);
            cl -> link = l;
            break;
    }
    return cl;
}

struct cont_list * CL_KWhileCons(struct cmd * c, struct cont_list *l)
{
    struct cont_list * cl = new_cont_list_ptr();
    cl -> c = c, cl -> link = l;
    return cl;
}

ResProg * InitResProg(GlobItemList * c) {
    ResProg * res = NewResProgPtr();
    res -> foc = c;
    res -> ectx = CL_Nil();
    var_state = init_SLL_hash();
    return res;
}

void step(ResProg * r) 
{
}

int TestEnd(ResProg * r) 
{
    if (r -> foc == nullptr && r -> ectx == nullptr) return 1;
    else return 0;
}
