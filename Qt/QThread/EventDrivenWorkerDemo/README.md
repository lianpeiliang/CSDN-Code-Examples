# EventDrivenWorkerDemo：事件驱动 Worker

这个示例演示如何让工作线程中的 Worker 使用自己的事件循环分步处理任务，并在步骤之间接收开始或取消请求。

## 运行效果

- 点击“开始约 10 秒任务”后，工作线程中的 `QTimer` 每 100 毫秒触发一次，只推进一个任务步骤。
- 进度条由 Worker 发出信号更新；GUI 主线程的心跳计数器持续增加。
- 点击“取消任务”后，取消请求排队到 Worker 所属的工作线程；Worker 在处理该请求时停止定时器并发出完成信号。
- 完成或取消后可以再次开始任务。

## 关键点

- `moveToThread()` 改变 Worker 的线程归属；`QThread` 对象本身仍属于创建它的 GUI 线程。
- `QThread::start()` 启动工作线程，默认 `QThread::run()` 运行该线程的事件循环。
- `QTimer` 在 Worker 的 `initialize()` 槽中创建，因此定时器属于工作线程。
- 定时器每次只调用一次短小的 `processOneStep()`。槽返回后，工作线程事件循环可以处理其他事件，包括排队到 Worker 的取消请求。
- GUI 与 Worker 通过跨线程信号槽通信，界面控件只在 GUI 线程访问。
- 这个示例没有在任务步骤中睡眠或执行长时间计算。若单步操作本身耗时很长，取消和其他事件仍要等该步骤返回后才能处理。
- 关闭窗口时，程序排队取消任务，然后退出并等待工作线程。

事件驱动方式适合需要在线程中接收多个请求，或使用 `QTimer`、`QTcpSocket`、`QProcess` 等事件驱动对象的工作。纯粹的单次阻塞计算也可以使用同步任务循环，但要设计好协作式取消和线程收尾。

## 项目文件

- `EventDrivenWorkerDemo.pro`：qmake 项目配置，使用 Qt Widgets 和 C++17。
- `main.cpp`：创建应用和窗口。
- `eventdrivenworkerdemo.h/.cpp`：界面、线程生命周期及信号槽连接。
- `eventworker.h/.cpp`：工作线程中的定时器和分步任务逻辑。

## 构建与运行

使用 Qt Creator 打开 `EventDrivenWorkerDemo.pro`，选择已安装的 Qt 工具链后构建并运行。也可以在配置好 Qt 工具链的命令行中执行：

```sh
qmake EventDrivenWorkerDemo.pro
make
```

Windows 的 MinGW 环境可使用 `mingw32-make` 替代 `make`；也可以使用 Qt Creator 配置的 MSVC 工具链构建。
