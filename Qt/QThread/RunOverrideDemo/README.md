# RunOverrideDemo：继承 `QThread` 并重写 `run()`

这个示例演示一种多线程写法：继承 `QThread`，在 `run()` 中执行一次耗时任务，并通过信号把进度发送给 GUI。

## 示例功能

- 点击“开始任务”后，`WorkThread::run()` 在工作线程中执行模拟任务。
- `run()` 发出 `progressChanged` 信号，进度条在 GUI 主线程更新。
- 点击“请求取消”后发送协作式中断请求，任务在检查点主动返回。
- 窗口析构时请求任务中断并等待线程退出。

## 关键点

- `QThread` 对象本身通常属于创建它的线程；调用 `start()` 后，Qt 在新线程中调用重写的 `run()`。
- 本例的耗时工作直接写在 `run()` 中，因此它确实在新线程执行。
- `run()` 没有调用 `exec()`，所以这条线程只执行这次任务，不启动事件循环。
- GUI 控件只在 GUI 主线程访问。工作线程通过信号发送进度，由 GUI 主线程更新进度条。
- `requestInterruption()` 不会强制终止任务；`run()` 必须主动检查 `isInterruptionRequested()`。
- 这种写法适合结构简单、单次执行的线程任务。如果工作对象需要在线程事件循环中接收多个排队槽、定时器或其他事件，通常使用独立的 `QObject` Worker 并将它移动到线程中。

## 项目文件

- `RunOverrideDemo.pro`：qmake 项目配置。
- `main.cpp`：创建应用和演示窗口。
- `workthread.h/.cpp`：继承 `QThread` 并重写 `run()` 的任务类。
- `runoverridedemo.h/.cpp`：界面、信号连接、启动和取消逻辑。
- [Knowledge/README.md：知识笔记目录](Knowledge/README.md)

## 构建与运行

使用 Qt Creator 打开 `RunOverrideDemo.pro`，选择已安装的 Qt 工具链后构建并运行。

也可以在配置好 Qt 工具链的命令行中执行：

```sh
qmake RunOverrideDemo.pro
make
```

Windows 的 MinGW 环境可使用 `mingw32-make` 替代 `make`。
