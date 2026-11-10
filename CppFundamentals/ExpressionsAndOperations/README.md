# 表达式求值与实际操作

本示例通过日志观察：一段表达式被求值时，是否创建了对象，是否调用了构造函数或赋值运算符。重点是理解为什么单独调用 `std::move` 不会完成移动，而使用它构造或赋值时可能发生移动。

## 文件说明

| 文件 | 用途 |
| --- | --- |
| [`main.cpp`](main.cpp) | 用 `Trace` 类输出默认构造、复制构造、移动构造、移动赋值和析构日志 |
| [`ExpressionsAndOperations.pro`](ExpressionsAndOperations.pro) | 可在 Qt Creator 中打开的 C++14 控制台项目 |

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

## 学习顺序

1. 先只看第一、第二阶段，理解单独调用 `std::move` 的效果。
2. 再对比第三、第四阶段，区分复制构造与移动构造。
3. 最后对比第四、第五阶段，区分创建对象与对已有对象赋值。

后续问答可继续记录在本目录的 `Knowledge` 子目录中。
