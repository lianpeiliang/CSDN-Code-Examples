# GuiBlockingDemo：主线程阻塞示例

这个示例演示 Qt GUI 程序在主线程执行耗时操作时的现象。计数器由 `QTimer` 每 100 毫秒更新一次；点击按钮后，程序在按钮回调中调用 `QThread::sleep(5)`，故意阻塞主线程 5 秒。

## 运行效果

- 空闲时，计数器持续增加。
- 点击“阻塞主线程 5 秒”后，主线程事件循环无法处理定时器、窗口重绘和用户输入，计数器会暂停，窗口也可能暂时无响应。
- 阻塞结束后，事件循环恢复，界面继续响应。

这是用于观察问题的演示代码。实际耗时工作不应放在 GUI 主线程中；后续可以用工作线程执行任务，并通过信号和槽更新界面。

## 项目文件

- `GuiBlockingDemo.pro`：qmake 项目配置，使用 Qt Widgets 和 C++17。
- `main.cpp`：示例程序入口与界面逻辑。

## 构建与运行

使用已安装 Qt（包含 Qt Widgets 和 qmake）的环境打开 `GuiBlockingDemo.pro`，例如在 Qt Creator 中打开后构建并运行。

也可以在配置好 Qt 工具链的命令行中执行：

```sh
qmake GuiBlockingDemo.pro
make
```

Windows 的 MinGW 环境可使用 `mingw32-make` 替代 `make`；也可以使用 Qt Creator 配置的 MSVC 工具链构建。
