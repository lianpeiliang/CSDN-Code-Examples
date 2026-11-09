#include "eventdrivenworkerdemo.h"

#include "eventworker.h"

#include <QLabel>
#include <QMetaObject>
#include <QProgressBar>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>

EventDrivenWorkerDemo::EventDrivenWorkerDemo(QWidget *parent)
    : QWidget(parent),
      worker(new EventWorker)
{
    setWindowTitle(QStringLiteral("事件驱动 Worker 示例"));
    resize(500, 300);

    auto *layout = new QVBoxLayout(this);
    auto *description = new QLabel(QStringLiteral(
        "Worker 在线程事件循环中用 QTimer 分步处理任务。\n"
        "任务运行时可以取消，也可以观察界面计数是否持续更新。"), this);
    statusLabel = new QLabel(QStringLiteral("状态：空闲"), this);
    heartbeatLabel = new QLabel(QStringLiteral("界面心跳：0"), this);
    progressBar = new QProgressBar(this);
    startButton = new QPushButton(QStringLiteral("开始约 10 秒任务"), this);
    cancelButton = new QPushButton(QStringLiteral("取消任务"), this);

    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    cancelButton->setEnabled(false);

    layout->addWidget(description);
    layout->addWidget(statusLabel);
    layout->addWidget(heartbeatLabel);
    layout->addWidget(progressBar);
    layout->addWidget(startButton);
    layout->addWidget(cancelButton);

    // 只移动 Worker。QThread 管理对象本身仍属于 GUI 线程。
    worker->moveToThread(&workerThread);

    connect(&workerThread, &QThread::started,
            worker, &EventWorker::initialize);
    connect(this, &EventDrivenWorkerDemo::startRequested,
            worker, &EventWorker::startTask);
    connect(this, &EventDrivenWorkerDemo::cancelRequested,
            worker, &EventWorker::cancelTask);
    connect(worker, &EventWorker::progressChanged,
            this, &EventDrivenWorkerDemo::updateProgress);
    connect(worker, &EventWorker::taskFinished,
            this, &EventDrivenWorkerDemo::onTaskFinished);
    connect(&workerThread, &QThread::finished,
            worker, &QObject::deleteLater);

    connect(startButton, &QPushButton::clicked,
            this, &EventDrivenWorkerDemo::startTask);
    connect(cancelButton, &QPushButton::clicked,
            this, &EventDrivenWorkerDemo::cancelTask);

    heartbeatTimer = new QTimer(this);
    connect(heartbeatTimer, &QTimer::timeout,
            this, &EventDrivenWorkerDemo::updateHeartbeat);
    heartbeatTimer->start(100);

    workerThread.start();
}

EventDrivenWorkerDemo::~EventDrivenWorkerDemo()
{
    if (workerThread.isRunning()) {
        // 任务每一步都会返回事件循环，因此取消请求可以在短时间内得到处理。
        QMetaObject::invokeMethod(worker, "cancelTask", Qt::BlockingQueuedConnection);
        workerThread.quit();
        workerThread.wait();
    }
}

void EventDrivenWorkerDemo::startTask()
{
    progressBar->setValue(0);
    statusLabel->setText(QStringLiteral("状态：运行中"));
    startButton->setEnabled(false);
    cancelButton->setEnabled(true);

    // 100 步，每步间隔 100 毫秒，任务总时长约 10 秒。
    emit startRequested(100, 100);
}

void EventDrivenWorkerDemo::cancelTask()
{
    statusLabel->setText(QStringLiteral("状态：正在取消……"));
    cancelButton->setEnabled(false);
    emit cancelRequested();
}

void EventDrivenWorkerDemo::updateProgress(int value)
{
    progressBar->setValue(value);
}

void EventDrivenWorkerDemo::onTaskFinished(bool cancelled)
{
    statusLabel->setText(cancelled ? QStringLiteral("状态：已取消")
                                   : QStringLiteral("状态：已完成"));
    startButton->setEnabled(true);
    cancelButton->setEnabled(false);
}

void EventDrivenWorkerDemo::updateHeartbeat()
{
    heartbeatLabel->setText(QStringLiteral("界面心跳：%1").arg(++heartbeatCount));
}
