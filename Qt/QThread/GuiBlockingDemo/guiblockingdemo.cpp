#include "guiblockingdemo.h"

#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QThread>
#include <QTimer>
#include <QVBoxLayout>

GuiBlockingDemo::GuiBlockingDemo(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(QStringLiteral("测试：主线程阻塞"));
    resize(420, 220);

    auto *layout = new QVBoxLayout(this);
    auto *description = new QLabel(QStringLiteral(
        "下面的计数器每 100 毫秒更新一次。\n"
        "点击按钮后，观察计数器以及窗口的响应。"), this);
    counterLabel = new QLabel(QStringLiteral("计数：0"), this);
    statusLabel = new QLabel(QStringLiteral("状态：空闲"), this);
    auto *button = new QPushButton(QStringLiteral("阻塞主线程 5 秒"), this);

    layout->addWidget(description);
    layout->addWidget(counterLabel);
    layout->addWidget(statusLabel);
    layout->addWidget(button);

    // 定时器和按钮回调都在 GUI 线程执行。
    auto *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GuiBlockingDemo::updateCounter);
    timer->start(100);

    connect(button, &QPushButton::clicked,
            this, &GuiBlockingDemo::blockMainThread);
}

void GuiBlockingDemo::updateCounter()
{
    counterLabel->setText(QStringLiteral("计数：%1").arg(++counterValue));
}

void GuiBlockingDemo::blockMainThread()
{
    qDebug() << "Blocking begins, thread:" << QThread::currentThread();
    statusLabel->setText(QStringLiteral("状态：正在执行耗时操作……"));

    // 故意阻塞 GUI 线程，观察事件循环无法处理重绘和输入的现象。
    QThread::sleep(5);

    statusLabel->setText(QStringLiteral("状态：操作完成"));
    qDebug() << "Blocking ends";
}
