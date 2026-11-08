#include <QApplication>
#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QThread>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QWidget window;
    window.setWindowTitle(QStringLiteral("测试：主线程阻塞"));
    window.resize(420, 220);
    auto *layout = new QVBoxLayout(&window);
    auto *description = new QLabel(QStringLiteral(
        "下面的计数器每 100 毫秒更新一次。\n"
        "点击按钮后，观察计数器以及窗口的响应。"), &window);
    auto *counterLabel = new QLabel(QStringLiteral("计数：0"), &window);
    auto *statusLabel = new QLabel(QStringLiteral("状态：空闲"), &window);
    auto *button = new QPushButton(QStringLiteral("阻塞主线程 5 秒"), &window);
    layout->addWidget(description);
    layout->addWidget(counterLabel);
    layout->addWidget(statusLabel);
    layout->addWidget(button);

    // 定时器和按钮槽都在主线程执行。
    auto *timer = new QTimer(&window);
    QObject::connect(timer, &QTimer::timeout, counterLabel,
                     [counterLabel, count = 0]() mutable {
        counterLabel->setText(QStringLiteral("计数：%1").arg(++count));
    });
    timer->start(100);

    QObject::connect(button, &QPushButton::clicked, &window, [statusLabel] {
        qDebug() << "Blocking begins, thread:" << QThread::currentThread();
        statusLabel->setText(QStringLiteral("状态：正在执行耗时操作……"));
        // 故意阻塞主线程，观察事件循环无法处理重绘和输入的现象。
        QThread::sleep(5);
        statusLabel->setText(QStringLiteral("状态：操作完成"));
        qDebug() << "Blocking ends";
    });
    window.show();
    return app.exec();
}
