#include "eventworker.h"

#include <QTimer>

EventWorker::EventWorker(QObject *parent)
    : QObject(parent)
{
}

void EventWorker::initialize()
{
    // 在 Worker 所属的工作线程中创建定时器，保证定时器和事件循环属于同一线程。
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout,
            this, &EventWorker::processOneStep);
}

void EventWorker::startTask(int requestedSteps, int intervalMs)
{
    if (running || timer == nullptr)
        return;

    currentStep = 0;
    totalSteps = qMax(1, requestedSteps);
    running = true;
    emit progressChanged(0);
    timer->start(qMax(1, intervalMs));
}

void EventWorker::cancelTask()
{
    if (!running)
        return;

    running = false;
    timer->stop();
    emit taskFinished(true);
}

void EventWorker::processOneStep()
{
    if (!running)
        return;

    ++currentStep;
    emit progressChanged(currentStep * 100 / totalSteps);

    if (currentStep >= totalSteps) {
        running = false;
        timer->stop();
        emit taskFinished(false);
    }
}
