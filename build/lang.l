%option noyywrap yylineno
%{
#include "lang.hpp"
#include "parser.hpp"
#include "lib.hpp"
%}

%%
0|[1-9][0-9]* {
    yylval.n = build_nat(yytext, yyleng);
    return TM_NAT;
}

-[1-9][0-9]* {
    yylval.n = build_neg_nat(yytext, yyleng);
    return TM_NAT;
}

"{" { return TM_LEFT_BRACE; }

"}" { return TM_RIGHT_BRACE; }

"(" { return TM_LEFT_PAREN; }

")" { return TM_RIGHT_PAREN; }

"<" { return TM_LT; }

"<=" { return TM_LE; }

">" { return TM_GT; }

">=" { return TM_GE; }

"==" { return TM_EQ; }

"!=" { return TM_NE; }

"+" { return TM_PLUS; }

"-" { return TM_MINUS; }

"*" { return TM_MUL; }

"/" { return TM_DIV; }

"%" { return TM_MOD; }

";" { return TM_SEMICOL; }

"," { return TM_COMMA; }

"=" { return TM_ASGNOP; }

"||" { return TM_OR; }

"&&" { return TM_AND; }

"!" { return TM_NOT; }

"&" { return TM_ADDR_OF; }

"int" { return TM_INT; }

"void" { return TM_VOID; }

"if" { return TM_IF; }

"then" { return TM_THEN; }

"else" { return TM_ELSE; }

"while" { return TM_WHILE; }

"do" { return TM_DO; }

"for" { return TM_FOR; }

"continue" { return TM_CON; }

"break" { return TM_BREAK; }

"return" { return TM_RET; }

"read_int" { return TM_RI; }

"write_char" { return TM_WC; }

"write_int" { return TM_WI; }

[_A-Za-z][_A-Za-z0-9]* {
    yylval.i = new_str(yytext, yyleng);
    return TM_IDENT;
}

[ \t\n\r] { };

%%