#include "workthread.h"

void WorkThread::run()
{
    // run() 在 start() 创建的工作线程中执行。
    // 这里不调用 exec()，因此本线程执行完任务后就会结束，不运行事件循环。
    for (int value = 0; value <= 100; value += 5) {
        if (isInterruptionRequested())
            return;

        emit progressChanged(value);

        if (value < 100)
            QThread::msleep(150);
    }
}
