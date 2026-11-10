# C++ 基础

本目录通过独立的小型示例学习 C++ 基础知识。每个专题包含可运行的源码、Qt Creator 可打开的 qmake 项目，以及围绕示例整理的学习问答。

建议先观察代码实际执行的操作，再结合问答理解对象生命周期、资源所有权、引用和参数传递等概念。

## 专题导航

| 专题 | 学习内容 | 源码 | 项目文件 |
| --- | --- | --- | --- |
| [对象生命周期与 RAII](ObjectLifetimeAndRAII/README.md) | 构造与析构、动态内存、智能指针、字符串移动、成员初始化 | [main.cpp](ObjectLifetimeAndRAII/main.cpp) | [ObjectLifetimeAndRAII.pro](ObjectLifetimeAndRAII/ObjectLifetimeAndRAII.pro) |
| [表达式求值与实际操作](ExpressionsAndOperations/README.md) | `std::move`、复制与移动、引用绑定、函数重载、按值与按引用传参 | [main.cpp](ExpressionsAndOperations/main.cpp) | [ExpressionsAndOperations.pro](ExpressionsAndOperations/ExpressionsAndOperations.pro) |

两个专题的 README 分别提供运行说明、预期输出、问答导读和复习要点。

## 建议学习顺序

1. 从[对象生命周期与 RAII](ObjectLifetimeAndRAII/README.md)开始，观察局部对象何时构造、何时析构。
2. 理解原始指针与被指向对象的区别，再学习 `unique_ptr` 如何管理动态对象。
3. 结合构造函数理解参数与成员、字符串复制与移动，以及成员初始化与赋值的区别。
4. 进入[表达式求值与实际操作](ExpressionsAndOperations/README.md)，区分类型转换、引用绑定、构造和赋值。
5. 结合函数参数继续分析：实参表达式的值类别与 `const` 限定，以及参数是引用还是独立对象。

## Knowledge 问答索引

问答中包含提问、书面化后的学习者回答，以及批注和讲解。部分回答保留了当时的疑问或错误判断，复习时应结合批注阅读。

| 所属专题 | 文档 | 主要内容 |
| --- | --- | --- |
| 对象生命周期与 RAII | [问答1](ObjectLifetimeAndRAII/Knowledge/问答1.md) | 作用域、析构、`new`/`delete`、悬空指针、RAII、所有权转移 |
| 对象生命周期与 RAII | [问答2](ObjectLifetimeAndRAII/Knowledge/问答2.md) | 临时对象、`unique_ptr` 的 `get()`、`release()` 与 `reset()` |
| 对象生命周期与 RAII | [问答3](ObjectLifetimeAndRAII/Knowledge/问答3.md) | 构造函数参数与成员、字符串移动、短字符串优化、内存占用 |
| 对象生命周期与 RAII | [问答4](ObjectLifetimeAndRAII/Knowledge/问答4.md) | 成员初始化列表、函数体内赋值、`const` 成员 |
| 对象生命周期与 RAII | [问答5](ObjectLifetimeAndRAII/Knowledge/问答5.md) | 成员初始化顺序、逆序析构、成员之间的依赖 |
| 表达式求值与实际操作 | [问答1](ExpressionsAndOperations/Knowledge/问答1.md) | 单独调用 `std::move`、引用绑定、具名右值引用与构造函数选择 |
| 表达式求值与实际操作 | [问答2](ExpressionsAndOperations/Knowledge/问答2.md) | 右值引用参数、函数重载、`std::move` 保留 `const` |
| 表达式求值与实际操作 | [问答3](ExpressionsAndOperations/Knowledge/问答3.md) | 按值与按引用传参、参数和局部对象的构造、源与目标的 `const` |

问答中的扩展代码片段不一定包含在各专题当前的 `main.cpp` 中，可按需要单独改写示例进行观察。

## 编译与运行

1. 选择一个专题，使用 Qt Creator 打开其中的 `.pro` 文件。
2. 选择可用的 Desktop Qt 构建套件，编译器需支持 C++14。
3. 构建并运行项目，将控制台日志与专题 README 中的预期输出对照。

当前两个项目通过 qmake 链接 Qt Core，示例源码本身只使用 C++ 标准库。具体文件和运行过程见各专题 README。

[返回仓库首页](../README.md)
