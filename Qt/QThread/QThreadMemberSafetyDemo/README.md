# QThreadMemberSafetyDemo：保护 `QThread` 子类的共享成员数据

这个独立示例演示一种容易忽略的情况：`QThread` 对象通常属于创建它的 GUI 主线程，但重写的 `run()` 在 `start()` 启动的工作线程中执行。两个线程可能访问同一个 `QThread` 子类成员数据，因此需要明确保护共享状态。

## 示例行为

- `CounterThread::run()` 在工作线程里递增成员变量 `counterValue`。
- GUI 主线程的定时器调用 `readCounter()` 读取同一个成员变量。
- 点击“重置计数”时，GUI 主线程调用 `resetCounter()` 写入该成员变量。
- 读、写、递增都使用同一把 `QMutex` 保护。

## 关键代码逻辑

```cpp
// 工作线程：写
{
    QMutexLocker locker(&counterMutex);
    ++counterValue;
}

// GUI 线程：读
int CounterThread::readCounter() const
{
    QMutexLocker locker(&counterMutex);
    return counterValue;
}

// GUI 线程：重置
void CounterThread::resetCounter()
{
    QMutexLocker locker(&counterMutex);
    counterValue = 0;
}
```

如果读、写、重置没有使用同一把锁，就会发生对共享数据的并发访问。在 C++ 中，对普通整数并发读写属于数据竞争，结果未定义；不能只依靠“计数看起来正常”来判断代码安全。

注意，普通的 `int` 成员变量本身没有 QObject 意义上的线程归属。它只是 `QThread` 对象里的数据；`run()` 和 GUI 线程中的槽都能访问它，是否安全取决于是否正确同步。

## 文件

- `QThreadMemberSafetyDemo.pro`：qmake 项目配置。
- `main.cpp`：应用入口。
- `counterthread.h/.cpp`：重写 `run()` 的线程类和受互斥锁保护的计数器。
- `qthreadmembersafetydemo.h/.cpp`：界面和 GUI 线程中的读取、重置逻辑。

## 构建与运行

使用 Qt Creator 打开 `QThreadMemberSafetyDemo.pro`，配置 Qt 工具链后构建并运行。
