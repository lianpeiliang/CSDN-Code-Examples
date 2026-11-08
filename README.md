# Code Examples

这里整理与 CSDN 文章和个人学习相关的代码示例。每个示例尽量独立成项目，并在对应目录中提供运行说明和知识笔记，方便阅读、构建和继续实验。

## Qt 多线程示例

目前的示例主要围绕 Qt GUI 线程、`QThread` 和信号槽的跨线程通信，位于 [`Qt/QThread`](Qt/QThread/)：

| 示例 | 内容 | 文档 |
| --- | --- | --- |
| `GuiBlockingDemo` | 在 GUI 主线程执行耗时操作时，观察事件循环停止处理、界面暂时无响应的现象。 | [项目说明](Qt/QThread/GuiBlockingDemo/README.md) · [知识笔记](Qt/QThread/GuiBlockingDemo/Knowledge/README.md) |
| `Multithreading` | 使用工作线程执行任务，通过信号更新进度，并演示协作式取消。 | [项目说明](Qt/QThread/Multithreading/README.md) · [知识问答](Qt/QThread/Multithreading/Knowledge/) |
| `RunOverrideDemo` | 继承 `QThread` 并重写 `run()`，在工作线程中执行一次性任务。 | [项目说明](Qt/QThread/RunOverrideDemo/README.md) · [知识问答](Qt/QThread/RunOverrideDemo/Knowledge/README.md) |
| `QThreadMemberSafetyDemo` | 演示 GUI 主线程与 `run()` 同时访问 `QThread` 子类成员数据时，如何用互斥锁保护共享状态。 | [项目说明](Qt/QThread/QThreadMemberSafetyDemo/README.md) |

### 建议阅读顺序

1. 从 `GuiBlockingDemo` 观察为什么耗时任务会让界面卡住。
2. 阅读 `Multithreading`，了解把任务放到工作线程以及通过信号槽与 GUI 通信。
3. 对比 `RunOverrideDemo`，理解继承 `QThread` 并重写 `run()` 的适用方式。
4. 阅读 `QThreadMemberSafetyDemo`，进一步理解线程归属和共享成员数据的线程安全。

## 构建与运行

示例项目的具体依赖和构建步骤请查看各自的 README。Qt 项目可以使用 Qt Creator 打开对应的 `.pro` 文件，配置可用的 Qt 与 C++ 工具链后构建运行。仓库保留源码、项目文件和 Markdown 说明；本地构建目录及 Qt Creator 用户配置不属于示例内容。
