#include "window.h"

#include "worker.h"

#include <QDebug>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>

Window::Window(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(QStringLiteral("工作线程：界面保持响应"));
    resize(460, 280);

    auto *layout = new QVBoxLayout(this);
    auto *description = new QLabel(QStringLiteral(
        "点击开始，在工作线程中执行约 5 秒的任务。\n"
        "观察计数器、进度条，并尝试移动窗口。"), this);
    counterLabel = new QLabel(QStringLiteral("界面计数：0"), this);
    statusLabel = new QLabel(QStringLiteral("状态：尚未开始"), this);
    progressBar = new QProgressBar(this);
    startButton = new QPushButton(QStringLiteral("开始任务（本次运行一次）"), this);
    cancelButton = new QPushButton(QStringLiteral("取消任务"), this);

    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    cancelButton->setEnabled(false);

    layout->addWidget(description);
    layout->addWidget(counterLabel);
    layout->addWidget(statusLabel);
    layout->addWidget(progressBar);
    layout->addWidget(startButton);
    layout->addWidget(cancelButton);

    auto *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Window::updateCounter);
    timer->start(100);

    connect(startButton, &QPushButton::clicked, this, &Window::startTask);
    connect(cancelButton, &QPushButton::clicked, this, &Window::cancelTask);

    qDebug() << "GUI thread:" << QThread::currentThread();
}

void Window::updateCounter()
{
    counterLabel->setText(QStringLiteral("界面计数：%1").arg(++counterValue));
}

void Window::startTask()
{
    startButton->setEnabled(false);
    cancelButton->setEnabled(true);
    statusLabel->setText(QStringLiteral("状态：执行中"));

    // Worker 无父对象，才能改变线程归属（thread affinity）。
    auto *worker = new Worker;
    worker->moveToThread(&thread);

    // start() 启动线程后会发出 started；Worker 已归属该线程，
    // 因此 doWork() 会在工作线程中执行，而不是 GUI 线程。
    connect(&thread, &QThread::started, worker, &Worker::doWork);

    // 默认 AutoConnection 会根据线程归属选择调用方式：这里跨线程，
    // 所以进度更新会排队到 GUI 线程，保证控件只由 GUI 线程访问。
    connect(worker, &Worker::progress,
            progressBar, &QProgressBar::setValue);

    // this 是 GUI 线程中的窗口对象，完成状态也会排队回 GUI 线程处理。
    connect(worker, &Worker::finished,
            this, &Window::onTaskFinished);

    // finished 后请求 QThread 的事件循环退出；quit() 不会强行中断正在运行的槽。
    connect(worker, &Worker::finished,
            &thread, &QThread::quit);

    // 在线程结束时安排 Worker 延迟销毁，避免在错误线程直接删除它。
    connect(&thread, &QThread::finished,
            worker, &QObject::deleteLater);

    // start() 才真正启动底层线程，并触发上面的 started 连接。
    thread.start();
}

void Window::cancelTask()
{
    // 线程安全的协作取消请求；它只设置请求标志，不会强行终止 Worker。
    // Worker 在任务循环中检查该标志后自行返回。
    thread.requestInterruption();
    statusLabel->setText(QStringLiteral("状态：正在取消……"));
    cancelButton->setEnabled(false);
}

void Window::onTaskFinished(bool cancelled)
{
    statusLabel->setText(cancelled ? QStringLiteral("状态：已取消")
                                   : QStringLiteral("状态：已完成"));
    cancelButton->setEnabled(false);
}

Window::~Window()
{
    // 析构时通知任务尽快结束，再退出线程事件循环并等待线程收尾，
    // 避免 QThread 仍运行时就销毁它。
    thread.requestInterruption();
    thread.quit();
    thread.wait();
}
