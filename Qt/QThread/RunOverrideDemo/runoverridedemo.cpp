#include "runoverridedemo.h"

#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QVBoxLayout>

RunOverrideDemo::RunOverrideDemo(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(QStringLiteral("继承 QThread 并重写 run()"));
    resize(460, 240);

    auto *layout = new QVBoxLayout(this);
    auto *description = new QLabel(QStringLiteral(
        "任务在 WorkThread::run() 中执行。\n"
        "观察进度更新时，窗口仍可响应。"), this);
    statusLabel = new QLabel(QStringLiteral("状态：空闲"), this);
    progressBar = new QProgressBar(this);
    startButton = new QPushButton(QStringLiteral("开始任务"), this);
    cancelButton = new QPushButton(QStringLiteral("请求取消"), this);

    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    cancelButton->setEnabled(false);

    layout->addWidget(description);
    layout->addWidget(statusLabel);
    layout->addWidget(progressBar);
    layout->addWidget(startButton);
    layout->addWidget(cancelButton);

    connect(startButton, &QPushButton::clicked,
            this, &RunOverrideDemo::startWork);
    connect(cancelButton, &QPushButton::clicked,
            this, &RunOverrideDemo::cancelWork);
    connect(&workerThread, &WorkThread::progressChanged,
            progressBar, &QProgressBar::setValue);
    connect(&workerThread, &QThread::finished,
            this, &RunOverrideDemo::onThreadFinished);
}

RunOverrideDemo::~RunOverrideDemo()
{
    workerThread.requestInterruption();
    workerThread.wait();
}

void RunOverrideDemo::startWork()
{
    cancellationRequested = false;
    progressBar->setValue(0);
    statusLabel->setText(QStringLiteral("状态：执行中"));
    startButton->setEnabled(false);
    cancelButton->setEnabled(true);

    workerThread.start();
}

void RunOverrideDemo::cancelWork()
{
    cancellationRequested = true;
    statusLabel->setText(QStringLiteral("状态：正在取消……"));
    cancelButton->setEnabled(false);

    // 中断请求只是通知；run() 会在下一次检查时主动返回。
    workerThread.requestInterruption();
}

void RunOverrideDemo::onThreadFinished()
{
    statusLabel->setText(cancellationRequested
                             ? QStringLiteral("状态：已取消")
                             : QStringLiteral("状态：已完成"));
    startButton->setEnabled(true);
    cancelButton->setEnabled(false);
}
