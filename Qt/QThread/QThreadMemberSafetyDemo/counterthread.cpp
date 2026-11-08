#include "counterthread.h"

#include <QMutexLocker>

CounterThread::CounterThread(QObject *parent)
    : QThread(parent)
{
}

int CounterThread::readCounter() const
{
    QMutexLocker locker(&counterMutex);
    return counterValue;
}

void CounterThread::resetCounter()
{
    // 这个槽由 GUI 线程调用；run() 也会访问 counterValue。
    // 同一把互斥锁保护两边对共享成员的访问。
    QMutexLocker locker(&counterMutex);
    counterValue = 0;
}

void CounterThread::run()
{
    while (!isInterruptionRequested()) {
        {
            QMutexLocker locker(&counterMutex);
            ++counterValue;
        }

        // 不要持锁睡眠，让 GUI 线程有机会读取或重置计数器。
        QThread::msleep(10);
    }
}
