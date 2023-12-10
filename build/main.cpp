#include <cstdio.h>
#include "lang.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "interpreter.hpp"

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
    ResProg * r = InitResProg(root);
    while (!TestEnd(r)) {
        Step(r);
    }
}
