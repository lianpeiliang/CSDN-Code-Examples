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

## 知识问答

`Knowledge` 目录记录了围绕本示例展开的提问、回答和补充解释，可以按编号顺序阅读：

1. [问答1：耗时槽为什么会延迟取消请求？](Knowledge/问答1.md)
2. [问答2：QTimer 如何让 Worker 分步执行任务？](Knowledge/问答2.md)
3. [问答3：Qt 为什么限制在定时器所属线程中启动和停止它？](Knowledge/问答3.md)
4. [问答4：停止定时器后，工作线程也会结束吗？](Knowledge/问答4.md)
5. [问答5：排队调用中的参数何时确定？](Knowledge/问答5.md)
6. [问答6：悬空指针与野指针有什么区别？](Knowledge/问答6.md)
7. [问答7：moveToThread() 与 start() 分别起什么作用？](Knowledge/问答7.md)
8. [问答8：线程发出 finished() 后，为什么还可能执行清理代码？](Knowledge/问答8.md)
9. [问答9：事件循环停止后，deleteLater() 为什么还能销毁 Worker？](Knowledge/问答9.md)
10. [问答10：将 finished() 到 deleteLater() 的连接改为排队连接会怎样？](Knowledge/问答10.md)
11. [问答11：关闭窗口时，为什么使用阻塞排队调用取消任务？](Knowledge/问答11.md)
12. [问答12：同一线程中的阻塞排队调用为什么会死锁？](Knowledge/问答12.md)
13. [问答13：不同线程之间的阻塞排队调用也会死锁吗？](Knowledge/问答13.md)
14. [问答14：成员指针指向的定时器会随 Worker 一起移动吗？](Knowledge/问答14.md)
15. [问答15：有父对象的 Worker 能单独移动到工作线程吗？](Knowledge/问答15.md)
16. [问答16：lambda 捕获 this 能保证在 GUI 线程执行吗？](Knowledge/问答16.md)

## 构建与运行

使用 Qt Creator 打开 `EventDrivenWorkerDemo.pro`，选择已安装的 Qt 工具链后构建并运行。也可以在配置好 Qt 工具链的命令行中执行：

```sh
qmake EventDrivenWorkerDemo.pro
make
```

Windows 的 MinGW 环境可使用 `mingw32-make` 替代 `make`；也可以使用 Qt Creator 配置的 MSVC 工具链构建。
