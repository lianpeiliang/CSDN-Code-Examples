# 对象生命周期与 RAII

这是一个用于学习 C++ 对象生命周期、资源所有权和成员初始化的控制台示例。建议先阅读并运行 [`main.cpp`](main.cpp)，再结合 `Knowledge` 目录中的问答逐步理解相关概念。

## 文件说明

| 文件 | 用途 |
| --- | --- |
| [`main.cpp`](main.cpp) | 可运行的示例，通过局部 `Worker` 对象观察构造和析构顺序 |
| [`ObjectLifetimeAndRAII.pro`](ObjectLifetimeAndRAII.pro) | 使用 C++14 的 qmake 控制台项目，可通过 Qt Creator 打开 |
| [`Knowledge/问答1.md`](Knowledge/问答1.md) | 对象生命周期、动态内存、RAII 与所有权转移 |
| [`Knowledge/问答2.md`](Knowledge/问答2.md) | 临时对象与 `unique_ptr` 的常用操作 |
| [`Knowledge/问答3.md`](Knowledge/问答3.md) | 构造函数参数、字符串移动与内存分配 |
| [`Knowledge/问答4.md`](Knowledge/问答4.md) | 成员初始化与构造函数体内赋值的区别 |

## 编译与运行

1. 使用 Qt Creator 打开 [`ObjectLifetimeAndRAII.pro`](ObjectLifetimeAndRAII.pro)。
2. 选择可用的 Desktop Qt 构建套件，编译器需支持 C++14。
3. 构建项目，然后运行控制台程序。

项目通过 qmake 配置并链接 Qt Core，示例源码本身只使用 C++ 标准库。

## 示例运行过程

当前 `main.cpp` 在内部大括号作用域中创建一个局部 `Worker` 对象。构造函数输出创建提示；离开该作用域时，对象自动析构，析构函数输出销毁提示。

预期输出：

```text
Entering scope
worker constructed
Working
worker destroyed
Left scope
```

构造函数还通过 `std::move`，用参数 `name` 移动构造成员字符串 `name_`。两者是不同的字符串对象；移动构造可以在适用时复用参数已有的资源。

## 问答阅读顺序

问答文档记录了提问、书面化后的学习者回答，以及对应的批注和讲解。复习时应结合批注阅读，区分原来的理解与后续修正。

1. [问答1：对象生命周期与 RAII](Knowledge/问答1.md)：作用域结束、逆序析构、提前返回、`new`/`delete`、悬空指针，以及 `unique_ptr` 的所有权转移。
2. [问答2：临时对象与所有权](Knowledge/问答2.md)：临时对象的析构时机、`get()` 返回的非拥有指针，以及 `release()` 与 `reset()` 的区别。
3. [问答3：参数与字符串移动](Knowledge/问答3.md)：参数与成员的区别、复制构造与移动构造、短字符串优化，以及构造过程中的内存占用。
4. [问答4：成员初始化与赋值](Knowledge/问答4.md)：通过初始化列表直接构造成员，与先默认构造再移动赋值的区别。

问答中的代码片段是在原示例基础上的扩展。智能指针相关示例位于问答文档中，当前 `main.cpp` 演示的是局部对象生命周期和字符串成员初始化。

## 与 RAII 的联系

RAII 是 **Resource Acquisition Is Initialization（资源获取即初始化）** 的缩写。它将资源的获取与管理对象的初始化关联起来，并在管理对象的析构函数中释放资源。对于具有自动存储期的局部管理对象，离开作用域时会自动析构，因此提前 `return` 也能触发资源清理。

例如，局部的 `std::unique_ptr<Worker>` 可以管理动态创建的 `Worker`。智能指针变量本身具有自动存储期，被管理的对象具有动态存储期。智能指针析构时，会销毁它拥有的对象并释放相应的存储空间。

## 练习建议

1. 运行 `main.cpp` 前，先预测完整的输出顺序。
2. 在同一个作用域中再创建一个 `Worker`，观察两个对象的析构顺序。
3. 增加一个嵌套作用域并在其中创建 `Worker`，判断每个对象的析构时机。
4. 在一个提前返回的函数中创建局部 `Worker`，观察对象是否仍会自动析构。
5. 结合[问答4](Knowledge/问答4.md)，比较 `name_(std::move(name))` 初始化与构造函数体内赋值的区别。
