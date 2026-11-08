# QThread 中断请求：`requestInterruption()` 与 `isInterruptionRequested()`

这两个方法配合起来实现**协作式取消**：一处发出中断请求，另一处检查请求。它们不会强制杀掉线程。

## 两个方法分别做什么

| 方法 | 作用 | 本项目中的调用位置 |
| --- | --- | --- |
| `requestInterruption()` | 向 `QThread` 管理的线程发出中断请求。它是一个通知，不会自动停止正在运行的代码或事件循环。 | `Window::cancelTask()` |
| `isInterruptionRequested()` | 读取当前线程是否收到中断请求。任务代码据此决定何时结束。 | `Worker::doWork()` |

`requestInterruption()` 可以从 GUI 线程调用；它是线程安全的。`isInterruptionRequested()` 应在被检查的线程内部调用，因此 Worker 使用 `QThread::currentThread()` 获取当前工作线程后再检查。

## 本项目中的用法

点击取消按钮时，窗口调用：

```cpp
void Window::cancelTask()
{
    thread.requestInterruption();
    statusLabel->setText(QStringLiteral("状态：正在取消……"));
    cancelButton->setEnabled(false);
}
```

工作线程在耗时任务的循环中定期检查：

```cpp
if (QThread::currentThread()->isInterruptionRequested()) {
    emit finished(true);
    return;
}
```

当请求尚未到达时，`isInterruptionRequested()` 返回 `false`，循环继续执行；读到 `true` 后，Worker 发出 `finished(true)` 并从任务函数返回。随后 `finished` 信号会通知窗口显示取消状态，并连接到 `QThread::quit()`，让工作线程的事件循环退出。

## 取消时机

`requestInterruption()` 不会中断 `QThread::msleep(100)`。如果用户在 Worker 睡眠期间点击取消，Worker 会先完成当前睡眠，并可能再发出一次进度信号，然后在下一次循环检查时发现请求并退出。检查点越频繁，任务通常越快响应取消；检查点应放在任务能够安全收尾的位置。

`quit()` 也不会强行打断正在执行的 `doWork()`。它用于退出线程的事件循环；当前任务函数返回后，线程才能完成退出。窗口析构时的 `thread.wait()` 会等待线程真正结束，避免销毁仍在运行的 `QThread`。

## 记忆方式

- `requestInterruption()`：**提出请求**。
- `isInterruptionRequested()`：**检查请求并决定是否收尾**。
- 任务代码必须主动检查；请求本身不是强制终止命令。

参考：[Qt QThread 文档：requestInterruption() 与 isInterruptionRequested()](https://doc.qt.io/qt-6/qthread.html#requestInterruption)
