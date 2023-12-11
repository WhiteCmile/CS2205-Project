#include <exception>
#include <iostream>
#include <cstdio>
#include "RE.hpp"
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
    try
    {
        PreProcess(root, table); // process the global items
        auto main_func = dynamic_cast<FuncPtr*>((table -> GetValue("main")).get());
        if (!main_func) 
            throw RuntimeError("main function is not defined");
        GlobItem * glob_main = main_func -> GetFunc();
        if (!glob_main || glob_main -> type != T_FUNC_DEF) 
            throw RuntimeError("error type of main function");
        Cmd * main_cmds = glob_main -> data.FUNC_DEF.body; // get body of main function
        ResProg * res = InitResProg(main_cmds);
        while (!TestEnd(res)) Step(res, table);
    }
    catch(const std :: exception & RE)
    {
        std :: cout << "Exception occured: " << RE.what() << std :: endl;
    }
    delete table;
    return 0;
}
