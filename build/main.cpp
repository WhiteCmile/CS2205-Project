#include <cstdio>
#include <exception>
#include "lang.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "interpreter.hpp"
#include "symboltable.hpp"

extern GlobItemList * root;
int yyparse();

int main(int argc, char * * argv) {
    if (argc == 1) {
        printf("Error, not enough arguments!\n");
        return 0;
    }
    if (argc >= 3) {
        printf("Error, too many arguments!\n");
        return 0;
    }
    yyin = fopen(argv[1], "rb");
    if (yyin == NULL) {
        printf("File %s can't be opened.\n", argv[1]);
        return 0;
    }
    yyparse();
    fclose(yyin);
    SymbolTable * table = new SymbolTable();
    PreProcess(root, table); // process the global items
    GlobItem * glob_main = (table -> GetValue("main")) -> GetFunc();
    if (!glob_main || glob_main -> type != T_FUNC_DEF) throw :: exception();
    Cmd * main_cmds = glob_main -> data.FUNC_DEF.body; // get body of main function
    ResProg * res = InitResProg(main_cmds);
    while (!TestEnd(res)) Step(res);
    delete table;
    return 0;
}
