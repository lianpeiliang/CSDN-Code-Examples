# 表达式求值与实际操作

本示例通过日志观察：一段表达式被求值时，是否创建了对象，是否调用了构造函数或赋值运算符。重点是理解为什么单独调用 `std::move` 不会完成移动，而使用它构造或赋值时可能发生移动。

## 文件说明

| 文件 | 用途 |
| --- | --- |
| [`main.cpp`](main.cpp) | 用 `Trace` 类输出默认构造、复制构造、移动构造、移动赋值和析构日志 |
| [`ExpressionsAndOperations.pro`](ExpressionsAndOperations.pro) | 可在 Qt Creator 中打开的 C++14 控制台项目 |
| [`Knowledge/问答1.md`](Knowledge/问答1.md) | 单独调用 `std::move`、引用绑定与构造函数选择 |
| [`Knowledge/问答2.md`](Knowledge/问答2.md) | 具名右值引用、函数重载与 `const` 对移动的影响 |
| [`Knowledge/问答3.md`](Knowledge/问答3.md) | 按值传参、按引用传参，以及参数与局部对象的构造 |

## 编译与运行

1. 使用 Qt Creator 打开 [`ExpressionsAndOperations.pro`](ExpressionsAndOperations.pro)。
2. 选择可用的 Desktop Qt 构建套件，编译器需支持 C++14。
3. 构建并运行，按阶段观察控制台输出。

项目通过 qmake 链接 Qt Core，示例源码本身只使用 C++ 标准库。

## 先理解“表达式”与“操作”的关系

表达式可以用来计算结果，也可以在求值过程中产生副作用，例如修改变量、调用构造函数或输出日志。因此，“表达式”和“实际操作”并不是互斥的两类东西。

阅读代码时，可以逐步判断：

1. 这段代码是否在创建新对象？
2. 如果对象已经存在，是否在对它赋值？
3. 表达式是否只是改变了类型或值类别，让后续操作可以选择不同的函数？
4. 最终调用了哪个构造函数、赋值运算符或其他函数？

## 示例中的五种情况

| 代码 | 在本例中发生的操作 |
| --- | --- |
| `Trace first;` | 创建 `first`，调用默认构造函数 |
| `(void)std::move(first);` | 求值后丢弃结果，不创建新的 `Trace`，也不调用其移动构造或移动赋值 |
| `Trace second(first);` | 创建 `second`，调用复制构造函数 |
| `Trace third(std::move(first));` | 创建 `third`，调用移动构造函数 |
| `second = std::move(third);` | 对已经存在的 `second` 赋值，调用移动赋值运算符 |

`std::move` 的作用是进行类型转换，使结果可用于选择移动操作；它本身不会创建一个新的 `Trace` 对象，也不会自动转移资源。真正执行什么操作，要看这个结果后续如何被使用，以及对应类型提供了哪些函数。

第二阶段的 `(void)` 表示主动丢弃表达式结果，用于避免某些编译器针对“结果未使用”的警告。它不会额外构造一个 `Trace` 对象。

本例中的 `Trace` 只记录函数调用，不管理动态资源，移动函数也没有实现资源转移或清空源对象的逻辑。实际类型在移动函数里做什么，由该类型的实现决定，不能据此认为任何对象经过移动后都会变空。

## 预期输出

```text
[1] Create first
default constructor
[2] Evaluate std::move(first) alone
[3] Copy-construct second
copy constructor
[4] Move-construct third
move constructor
[5] Move-assign to second
move assignment
[6] Leave scope
destructor
destructor
destructor
```

第二阶段之后没有出现移动日志，因为没有使用表达式结果构造或赋值给其他对象。第五阶段只有赋值日志，因为 `second` 已经存在。

函数结束时，三个局部对象按 `third`、`second`、`first` 的顺序析构。本例的析构日志没有区分对象名称，因此最后三行内容相同。

## Knowledge 问答导读

`Knowledge` 记录了提问、书面化后的学习者回答，以及批注和讲解。部分回答保留了当时的不确定或错误判断，复习时需要结合批注阅读。

这些问答在 `main.cpp` 的基础上扩展了引用和函数参数场景，相关代码片段不一定出现在当前可运行示例中。

| 文档 | 主要内容 | 复习时关注的问题 |
| --- | --- | --- |
| [问答1：表达式与构造函数选择](Knowledge/问答1.md) | `std::move` 的转换、丢弃表达式结果、复制与移动构造、右值引用绑定 | 为什么 `Trace&& ref = std::move(first);` 不创建新的 `Trace`？为什么 `Trace third(ref);` 调用复制构造？ |
| [问答2：具名右值引用与函数参数](Knowledge/问答2.md) | 右值引用参数、函数重载选择、引用绑定与实际移动、`std::move` 保留 `const` | 为什么进入右值引用重载不等于执行移动构造？为什么对 `const` 对象使用 `std::move` 仍可能复制？ |
| [问答3：按值传参时的对象构造](Knowledge/问答3.md) | 按值参数与引用参数的区别、参数和局部对象分别初始化、源与目标上的 `const` | 为什么 `const Trace item` 可以由移动构造创建，但随后用 `std::move(item)` 初始化局部对象却会复制？ |

## 复习要点

### 引用类型与表达式的值类别需要分开判断

```cpp
Trace first;
Trace&& ref = std::move(first);

Trace a(ref);             // ref 是左值表达式，复制构造
Trace b(std::move(ref));  // std::move(ref) 是右值表达式，移动构造
```

`ref` 声明为右值引用，但直接使用名字 `ref` 时，表达式是左值。构造函数选择要看使用处的表达式，不能仅根据变量声明中的 `&&` 判断。

### 引用绑定与按值参数构造是不同的操作

分别考虑以下两种函数参数声明，并传入非 `const` 对象的 `std::move(first)`：

| 参数声明 | 调用时发生的操作 |
| --- | --- |
| `Trace&& item` | 引用绑定到原来的 `first`，不创建新的 `Trace` |
| `Trace item` | 创建独立的参数对象 `item`，调用本例的移动构造函数 |

按值参数构造完成后，直接使用名字 `item` 仍然是左值表达式。参数之前通过移动构造创建，不意味着后续使用它时都会自动发生移动。

### 区分目标对象的 `const` 与源表达式的 `const`

```cpp
void process(const Trace item) {
    Trace local(std::move(item));
}

// first 是已经存在的非 const Trace 对象
process(std::move(first));
```

初始化参数 `item` 时，源表达式 `std::move(first)` 不带 `const`，因此可以调用移动构造。目标参数为 `const`，允许在构造时获得初始状态。

初始化 `local` 时，源对象 `item` 带有 `const`。`std::move(item)` 保留这个限定，返回 `const Trace&&`，不能匹配本例的 `Trace(Trace&&)`，因此选择复制构造 `Trace(const Trace&)`。

`const Trace&` 参数还可以直接绑定到右值，这一步是引用绑定，不需要另外执行一次强制类型转换，也不因此创建新的 `Trace` 对象。

## 学习顺序

1. 先只看第一、第二阶段，理解单独调用 `std::move` 的效果。
2. 再对比第三、第四阶段，区分复制构造与移动构造。
3. 对比第四、第五阶段，区分创建对象与对已有对象赋值。
4. 阅读[问答1](Knowledge/问答1.md)，区分表达式转换、引用绑定和对象构造。
5. 阅读[问答2](Knowledge/问答2.md)，练习判断具名右值引用、函数重载和 `const` 的影响。
6. 阅读[问答3](Knowledge/问答3.md)，逐步分析传参和函数体内每一次初始化实际执行的操作。

复习时，建议按“源表达式的值类别与 `const` 限定 → 目标是引用还是独立对象 → 可用的构造或赋值函数”逐步判断。
