# Multithreading：工作线程示例

这个示例演示如何把耗时任务放到 Qt 工作线程中执行，让 GUI 主线程继续处理界面事件。

## 示例功能

- 点击“开始任务”后，在工作线程中执行约 5 秒的模拟任务。
- 工作线程通过信号发送进度，界面上的进度条随之更新。
- 界面计数器持续更新；任务运行时，窗口仍可移动和响应输入。
- 点击“取消任务”会发送协作式中断请求。工作线程检查请求后停止任务并通知界面。
- 窗口析构时会请求线程退出并等待线程结束，避免销毁仍在运行的 `QThread`。

## 项目文件

- `Multithreading.pro`：qmake 项目配置，使用 Qt Widgets 和 C++17。
- `main.cpp`：创建应用和主窗口。
- `window.h`、`window.cpp`：`initUI()` 创建窗口控件和布局；窗口槽函数负责计数更新、启动、取消和完成处理，线程启动与取消逻辑也在这里。
- `worker.h`、`worker.cpp`：工作线程中运行的任务，以及进度和完成信号。
- [Knowledge/问答1.md：`moveToThread()` 与事件循环](Knowledge/问答1.md)
- [Knowledge/问答2.md：中断请求](Knowledge/问答2.md)
- [Knowledge/问答3.md：`quit()` 与 `wait()`](Knowledge/问答3.md)

`Worker` 声明在独立头文件中，qmake 会自动为其中的 `Q_OBJECT` 调用 moc。

## 构建与运行

使用已安装 Qt（包含 Qt Widgets 和 qmake）的环境，在 Qt Creator 中打开 `Multithreading.pro`，配置工具链后构建并运行。

也可以在配置好 Qt 工具链的命令行中执行：

```sh
qmake Multithreading.pro
make
```

Windows 的 MinGW 环境可以使用 `mingw32-make`；也可以通过 Qt Creator 使用 MSVC 工具链构建。
