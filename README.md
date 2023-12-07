# 介绍

CS2205 大作业， 选题为: 在 WhileF 语言的基础上实现一个有基本类型系统和函数指针的程序语言解释器。

整个项目分为三个部分：
- 语法设计
- 前端实现（lexer and parser）
- 解释器

# 语法设计

WhileF 语言的语法为：

```
E :=    N | V | -E | E + E | E - E | E * E | E / E | E % E |
        E < E | E <= E | E == E | E != E | E >= E | E > E | 
        E && E | E || E | !E |
        * E | & E |
        FUNC_NAME (E, E, ..., E) | FUNC_NAME ()

C :=    var V; C |
        E = E |
        C; C |
        if (E) then { C } else { C } |
        while (E) do { C } |
        for { C } ( E ) { C } { C } |
        do { C } while (E) |
        PROC_NAME (E, E, ..., E) | PROC_NAME ()
        continue | break | return

GLOB_ITEM :=    var V |
                func FUNC_NAME (V, V, ..., V) { C } |
                proc PROC_NAME (V, V, ..., V) { C }

P :=    GLOB_ITEM ; GLOB_ITEM ; ... ; GLOB_ITEM
```

在加入基本类型系统与函数指针以及整数及字符串的输入输出后， 语法变为：

```
E :=    N | V | -E | E + E | E - E | E * E | E / E | E % E |
        E < E | E <= E | E == E | E != E | E >= E | E > E | 
        E && E | E || E | !E |
        * E | & E |
        read_int () | 
        V (E, E, ..., E) | V ()

STAR_LIST :=    * | STAR_LIST *

TYPE := int | int STAR_LIST |
        void |
        TYPE (STAR_LIST) () | TYPE (STAR_LIST) (TYPE, TYPE, ..., TYPE)

VAR_DECL :=     TYPE V |
                TYPE (STAR_LIST V) () | TYPE (STAR_LIST V) (TYPE, TYPE, ..., TYPE)

C :=    VAR_DECL |
        E = E |
        C; C |
        if (E) then { C } else { C } |
        while (E) do { C } |
        for { C } ( E ) { C } { C } |
        do { C } while (E) |
        V (E, E, ..., E) | V ()
        write_char ( E ) | write_int ( E )
        continue | break | return

GLOB_ITEM :=    VAR_DECL |
                TYPE FUNC_NAME (VAR_DECL, VAR_DECL, ..., VAR_DECL) { C } |
                TYPE FUNC_NAME () { C }

P :=    GLOB_ITEM ; GLOB_ITEM ; ... ; GLOB_ITEM
```

# 前端实现


# 解释器

