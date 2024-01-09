# 介绍

CS2205 大作业， 选题为: 在 WhileF 语言的基础上实现一个有基本类型系统和函数指针的程序语言解释器。

整个项目分为三个部分：
- 语法设计
- 前端实现（lexer and parser）
- 解释器

# 编译相关

要编译此项目， 需要创建 `./bin` 子目录， 之后使用 
```
make
```
编译即可

# 语法

## 语法设计

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
        E (E, E, ..., E) | E ()

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
        E (E, E, ..., E) | E ()
        write_char ( E ) | write_int ( E )
        continue | break | return

GLOB_ITEM :=    VAR_DECL |
                TYPE FUNC_NAME (VAR_DECL, VAR_DECL, ..., VAR_DECL) { C } |
                TYPE FUNC_NAME () { C }

P :=    GLOB_ITEM ; GLOB_ITEM ; ... ; GLOB_ITEM
```

## 语言特性

- 变量未定义时无法直接作为右值使用
- `&x` 事实上返回的是 `x` 更高一阶的指针
- 禁止类型转换， 所以没有 `void*` 这种万能指针
- 对于函数， 可在函数体中使用 `__return = <expr>` 来确定返回值， 返回值存储在 `__return` 中， 控制流的返回交由 `return` 语句实现； `__return` 的类型为函数的返回类型
- 若函数的返回类型为 `void` 且函数体中有 `__return = <expr>` 语句则发生报错， 因为此时会认为 `__return` 在函数所在的作用域中未定义
- 对于函数指针， 我们认为程序中定义的函数的函数名是一个 $0$ 级的函数指针， 要调用一个函数我们也需要 $0$ 级的函数指针来调用， 例如， 若我们定义了一个函数 `g`， 同时定义了一个 $1$ 级的函数指针 `f` 使得 `f = &g`， 那么要利用 `f` 调用 `g`， 我们需要使用 `(*f)()` 而不是 `f()`， 这点与 C 标准不同

# 前端实现

- frame(帧)
符号表：
记录了作用域与变量的信息的数据结构，解释器通过手动创建帧与删除帧来维护作用域的信息。
![](https://notes.sjtu.edu.cn/uploads/upload_2a5c0b861d3f27a61794730fed4c1fdf.png)

以下四个表格记录关于变量的全部信息，当通过名字访问变量时，先找到最晚定义的变量的时间判断是否合法，再到time表格中找到对应信息存放位置进行保存或修改。当通过地址访问时，我们只判断该地址是否存在对应变量，若存在则可以访问与修改，无关作用域。
![](https://)![](https://notes.sjtu.edu.cn/uploads/upload_751925b4f5dccd93e15fead1108ab171.png)

# 解释器
一些说明：
- 我们设计的解释器实现了int类型，函数可以定义int，也可以定义为void，但不支持类型转换。
- 我们支持函数调用，但递归层数不能过于庞大（会导致爆栈）。
- 对于for循环，for{c1}(e){c2}{c3}我们假设break、continue和return只能出现在c2或者c3中，不能出现在c1中。
- 对于while循环和do-while循环，break、continue和return出现位置无限制。
- 关于函数调用，我们需要明确调用函数后程序运行时在哪些函数体中以及函数的包含关系是怎么样的。我们给函数调用一个标记frame，函数调用的时候call一下frame，表明程序在这个函数层内，离开函数时把对应frame删去。同样，在for循环、while循环和do-while循环中也需要明确调用关系，也采用frame的方式。

- break与continue的处理：
    我们将循环语句全部区分为第一次进入与非第一次进入，每当我们需要进行break或continue时，我们不断向后搜索直到当前语句时“已经进入过的循环语句”，则退出该作用域即可。另外，在向后搜索时需要执行沿途所有的deleteframe语句。

- 余下均按照大作业要求完成

下面是该解释器采用的小步语义：
一、表达式相关的小步语义

- 变量、加法的小步语义
```
– 如果 n = s(x)，那么 (x, ϵ) -> (n, ϵ) @ s
– (e1 + e2, ϵ) -> (e1, KBinopL(+, e2)) @ s
– (n1, KBinopL(+, e2)) -> (e2, KBinopR(n1, +)) @ s
– 如果 n = n1 + n2 并且 $-2^{63} <= n <= 2^{63}-1$，那么 (n2, KBinopR(n1, +)) -> (n, ϵ) @ s
```

- 减法的小步语义：

```
– (e1 - e2, ϵ) -> (e1, KBinopL(-, e2)) @ s
– (n1, KBinopL(-, e2)) -> (e2, KBinopR(n1, -)) @ s
– 如果 n = n1 - n2 并且 $-2^{63} <= n <= 2^{63}-1$，那么 (n2, KBinopR(n1, -)) -> (n, ϵ) @ s
```

- 乘法的小步语义：
```
– (e1 * e2, ϵ) -> (e1, KBinopL(*, e2)) @ s
– (n1, KBinopL(*, e2)) -> (e2, KBinopR(n1, *)) @ s
– 如果 n = n1 * n2 并且 $-2^{63} <= n <= 2^{63}-1$，那么 (n2, KBinopR(n1, *) -> (n, ϵ) @ s
```

- 除法\取模的小步语义：（取模即 “\” 变为 “%”）
```
– (e1 / e2, ϵ) -> (e1, KBinopL(/, e2)) @ s
– (n1, KBinopL(/, e2)) -> (e2, KBinopR(n1, /)) @ s
– 如果n2 != 0, n = n1 / n2 并且 $-2^{63} <= n <= 2^{63}-1$，那么 (n2, KBinopR(n1, /) -> (n, ϵ) @ s
```

- 小于的小步语义：
```
– (e1 < e2, ϵ) -> (e1, KBinopL(<, e2)) @ s
– (n1, KBinopL(<, e2)) -> (e2, KBinopR(n1, <)) @ s
– 如果n1 < n2，那么 (n2, KBinopR(n1, <) -> (1, ϵ) @ s
– 否则，那么 (n2, KBinopR(n1, <) -> (0, ϵ) @ s
```

- 小于等于的小步语义：
```
– (e1 <= e2, ϵ) -> (e1, KBinopL(<=, e2)) @ s
– (n1, KBinopL(<=, e2)) -> (e2, KBinopR(n1, <=)) @ s
– 如果n1 <= n2，那么 (n2, KBinopR(n1, <=) -> (1, ϵ) @ s
– 否则，那么 (n2, KBinopR(n1, <=) -> (0, ϵ) @ s
```
- 大于的小步语义：
```
– (e1 > e2, ϵ) -> (e1, KBinopL(>, e2)) @ s
– (n1, KBinopL(>, e2)) -> (e2, KBinopR(n1, >)) @ s
– 如果n1 > n2，那么 (n2, KBinopR(n1, >) -> (1, ϵ) @ s
– 否则，那么 (n2, KBinopR(n1, >) -> (0, ϵ) @ s
```

- 大于等于的小步语义：
```
– (e1 >= e2, ϵ) -> (e1, KBinopL(>=, e2)) @ s
– (n1, KBinopL(>=, e2)) -> (e2, KBinopR(n1, >=)) @ s
– 如果n1 >= n2，那么 (n2, KBinopR(n1, >=) -> (1, ϵ) @ s
– 否则，那么 (n2, KBinopR(n1, >=) -> (0, ϵ) @ s
```

- 判等的小步语义：
```
– (e1 == e2, ϵ) -> (e1, KBinopL(==, e2)) @ s
– (n1, KBinopL(==, e2)) -> (e2, KBinopR(n1, ==)) @ s
– 如果n1 = n2，那么 (n2, KBinopR(n1, ==) -> (1, ϵ) @ s
– 否则，那么 (n2, KBinopR(n1, ==) -> (0, ϵ) @ s
```

- 判别不等于的小步语义：
```
– (e1 != e2, ϵ) -> (e1, KBinopL(!=, e2)) @ s
– (n1, KBinopL(!=, e2)) -> (e2, KBinopR(n1, !=)) @ s
– 如果n1 != n2，那么 (n2, KBinopR(n1, !=) -> (1, ϵ) @ s
– 否则，那么 (n2, KBinopR(n1, !=) -> (0, ϵ) @ s
```

且 和 或 采用短路求值
- 且的小步语义：
```
– (e1 && e2, ϵ) -> (e1, KBinopL(&&, e2)) @ s
– 如果 n1 = 0，(n1, KBinopL(op, e2)) -> (0, ϵ) @ s
– 如果 n1 != 0，(n1, KBinopL(op, e2)) -> (e2, KBinopR(n1, &&)) @ s
– 如果n2 = 0，那么 (n2, KBinopR(n1, &&) -> (0, ϵ) @ s
– 否则，(n2, KBinopR(n1, &&) -> (1, ϵ) @ s
```

- 或的小步语义：
```
– (e1 || e2, ϵ) -> (e1, KBinopL(||, e2)) @ s
- 如果 n1 = 1，(n1, KBinopL(op, e2)) -> (1, ϵ) @ s
– 如果 n1 != 1，(n1, KBinopL(op, e2)) -> (e2, KBinopR(n1, &&)) @ s
– 如果n2 = 1，那么 (n2, KBinopR(n1, ||) -> (1, ϵ) @ s
– 否则, (n2, KBinopR(n1, ||) -> (0, ϵ) @ s
```

一元运算符的小步语义：
- （1）取相反数：
```
– (-e, ϵ) -> (e,KUnOp(-)) @ s
– (n,KUnOp(-)) -> (-n, ϵ) @ s
```
- （2）非：
```
– (!e, ϵ) -> (e,KUnOp(!)) @ s
– 如果n = 0,(0,KUnOp(!)) -> (1, ϵ) @ s
– 否则 (n,KUnOp(!)) -> (0, ϵ) @ s
```
- （3）指针：
```
– (*e, ϵ) -> (e,KUnOp(*)) @ s
– (n, KUnOp(*)) -> (n, ϵ) @ s
```

- （4）取地址：
```
– (&e, ϵ) -> (e,KUnOp(&)) @ s
– (n, KUnOp(&)) -> (n, ϵ) @ s
```

- 读值read_int()的小步语义：
```
– (read_int(), ϵ) -> (n,KUnOp(&)) @ s
```

二、程序语句相关的小步语义
- 顺序语句：
```
(c1;c2,ϵ,s) -> (c1,KSeq(c2),s)
(ϵ,KSeq(c),s) -> (c,ϵ,s)
```

- if条件语句：
```
(if (e) then {c1} else {c2},ϵ,s) ->(NewFrame(), IF(e,c1,c2) \circ DeleteFrame()) ->(e,KIf(c1,c2),s)
如果 n = 0 (0,KIf(c1,c2),s) -> (c2,ϵ,s)
否则 (n,KIf(c1,c2),s) -> (c1,ϵ,s)
```

- while-do循环：
```
(while (e) do {c},ϵ,s) ->(NewFrame(), WhileDo(e,c) \circ DeleteFrame()) ->(e,KWhileDoCond(e,c),s)
如果 n != 0,(n,KWhileDoCond(e,c),s) -> (c,KWhileDoBody(e,c),s) 
否则 (0,KWhileDoCond(e,c),s) -> (ϵ,ϵ,s)
(ϵ,KWhileDoBody(e,c),s) -> (e,KWhileDoCond(e,c),s)
```

- for循环：
```
(for { c1 } ( e ) { c2 } { c3 },ϵ,s) -> 
(NewFrame(), For(c1, e, c2, c3) \circ DeleteFrame()) -> (c1,KForIncr(e,c2,c3),s) 
如果(ϵ,KForIncr(e,c2,c3),s) -> Eval(e) 为true则(c3, KForBody(e,c2,c3), s)
否则false时 (0,KForCond(e,c2,c3),s) -> (ϵ,ϵ,s)
(ϵ,KForBody(e,c2,c3),s) ->(c2,KForIncr(e,c2,c3),s)
```

- do-while-循环：
```
(do { c } while (e)) -> (NewFrame(), DoWhile(c,e) \circ DeleteFrame()) -> (c,KDoWhileBody(c,e),s)
(ϵ,KDoWhileBody(c,e),s) -> (e,KDoWhileCond(c,e),s)
如果 n != 0, (n,KDoWhileCond(c,e),s) -> (c,KDoWhileBody(c,e),s)
否则 (0,KDoWhileCond(c,e),s) -> (ϵ,ϵ,s)
(ϵ,KDoWhileBody(c,e),s) -> (e,KDoWhileCond(c,e),s)
```

- 函数调用：
```
(E(e, e, ..., e),ϵ,s) -> (NewFrame(), Func(E(e,e,...,e)) \circ DeleteFrame()) -> (KFunc(E,e,e,...,e),ϵ,s)
(KFunc(E,e,e,...,e),ϵ,s) -> 逐个求值(ϵ,KFuncBody(E,n1,n2,...,nk),s)
```

- 写值：
```
(write_char(e),ϵ,s) -> (e,ϵ,s)
(write_int(),ϵ,s) -> (n,ϵ,s)
```

- 赋值语句的小步语义：
```
- (x = e, ϵ, s) -> (e, KAsgnVar(x), s)
- (e, KAsgnVar(x), s) -> (n, KAsgnVar(x), s)
- (n, KAsgnVar(x), s) -> (ϵ, ϵ, s')
其中 s'(x) = n, 对于 y != x, s'(y) = s(y)
```



