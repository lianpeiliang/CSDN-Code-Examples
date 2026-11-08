#include "qthreadmembersafetydemo.h"

#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>

QThreadMemberSafetyDemo::QThreadMemberSafetyDemo(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(QStringLiteral("QThread 子类成员数据的线程安全"));
    resize(520, 300);

    auto *layout = new QVBoxLayout(this);
    auto *description = new QLabel(QStringLiteral(
        "run() 在工作线程递增 CounterThread 的成员变量；\n"
        "界面定时读取它，按钮也可以在运行时重置它。"), this);
    counterLabel = new QLabel(QStringLiteral("共享计数：0"), this);
    statusLabel = new QLabel(QStringLiteral("状态：线程未启动"), this);
    startButton = new QPushButton(QStringLiteral("启动计数线程"), this);
    stopButton = new QPushButton(QStringLiteral("停止计数线程"), this);
    resetButton = new QPushButton(QStringLiteral("重置计数"), this);

    stopButton->setEnabled(false);

    layout->addWidget(description);
    layout->addWidget(counterLabel);
    layout->addWidget(statusLabel);
    layout->addWidget(startButton);
    layout->addWidget(stopButton);
    layout->addWidget(resetButton);

    displayTimer = new QTimer(this);
    displayTimer->setInterval(100);
    connect(displayTimer, &QTimer::timeout,
            this, &QThreadMemberSafetyDemo::refreshCounter);
    displayTimer->start();

    connect(startButton, &QPushButton::clicked,
            this, &QThreadMemberSafetyDemo::startCounter);
    connect(stopButton, &QPushButton::clicked,
            this, &QThreadMemberSafetyDemo::stopCounter);
    connect(resetButton, &QPushButton::clicked,
            &counterThread, &CounterThread::resetCounter);
    connect(&counterThread, &QThread::finished,
            this, &QThreadMemberSafetyDemo::onThreadFinished);
}

QThreadMemberSafetyDemo::~QThreadMemberSafetyDemo()
{
    counterThread.requestInterruption();
    counterThread.wait();
}

void QThreadMemberSafetyDemo::startCounter()
{
    if (counterThread.isRunning())
        return;

    statusLabel->setText(QStringLiteral("状态：运行中"));
    startButton->setEnabled(false);
    stopButton->setEnabled(true);
    counterThread.start();
}

void QThreadMemberSafetyDemo::stopCounter()
{
    if (!counterThread.isRunning())
        return;

    statusLabel->setText(QStringLiteral("状态：正在停止……"));
    stopButton->setEnabled(false);
    counterThread.requestInterruption();
}

void QThreadMemberSafetyDemo::refreshCounter()
{
    // 此函数在 GUI 主线程执行；readCounter() 与 run() 共享 counterMutex。
    counterLabel->setText(QStringLiteral("共享计数：%1")
                              .arg(counterThread.readCounter()));
}

void QThreadMemberSafetyDemo::onThreadFinished()
{
    // finished() 发出时线程可能还在做最后清理；wait() 确保完全结束后再允许重启。
    counterThread.wait();
    statusLabel->setText(QStringLiteral("状态：线程已结束"));
    startButton->setEnabled(true);
    stopButton->setEnabled(false);
}
