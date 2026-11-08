#include "worker.h"

#include <QDebug>
#include <QThread>

void Worker::doWork()
{
    qDebug() << "Worker thread:" << QThread::currentThread();

    // 50 个小步骤模拟约 5 秒的耗时任务。
    for (int i = 1; i <= 50; ++i) {
        // requestInterruption() 只是发出协作式请求，不会强行终止线程；
        // Worker 需要定期检查并自行结束任务。
        // isInterruptionRequested() 只读取当前线程的请求状态，不会清除它，也不会自动停止任务。
        if (QThread::currentThread()->isInterruptionRequested()) {
            emit finished(true);
            return;
        }

        QThread::msleep(100);
        // 信号在工作线程发出；接收对象在 GUI 线程，Qt 会排队交付。
        emit progress(i * 2);
    }

    emit finished(QThread::currentThread()->isInterruptionRequested());
}
