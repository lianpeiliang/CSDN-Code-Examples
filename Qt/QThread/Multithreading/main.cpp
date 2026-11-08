#include <QApplication>
#include <QDebug>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QThread>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

class Worker : public QObject
{
    Q_OBJECT
public slots:
    void doWork()
    {
        qDebug() << "Worker thread:" << QThread::currentThread();
        // 50 个小步骤模拟约 5 秒的耗时任务。
        for (int i = 1; i <= 50; ++i) {
            if (QThread::currentThread()->isInterruptionRequested()) {
                emit finished(true);
                return;
            }
            QThread::msleep(100);
            emit progress(i * 2);
        }
        emit finished(QThread::currentThread()->isInterruptionRequested());
    }
signals:
    void progress(int value);
    void finished(bool cancelled);
};

class Window : public QWidget
{
public:
    Window()
    {
        setWindowTitle(QStringLiteral("工作线程：界面保持响应"));
        resize(460, 280);
        auto *layout = new QVBoxLayout(this);
        auto *description = new QLabel(QStringLiteral(
            "点击开始，在工作线程中执行约 5 秒的任务。\n"
            "观察计数器、进度条，并尝试移动窗口。"), this);
        auto *counter = new QLabel(QStringLiteral("界面计数：0"), this);
        auto *status = new QLabel(QStringLiteral("状态：尚未开始"), this);
        auto *bar = new QProgressBar(this);
        auto *start = new QPushButton(QStringLiteral("开始任务（本次运行一次）"), this);
        auto *cancel = new QPushButton(QStringLiteral("取消任务"), this);
        bar->setRange(0, 100);
        bar->setValue(0);
        cancel->setEnabled(false);
        layout->addWidget(description);
        layout->addWidget(counter);
        layout->addWidget(status);
        layout->addWidget(bar);
        layout->addWidget(start);
        layout->addWidget(cancel);

        auto *timer = new QTimer(this);
        QObject::connect(timer, &QTimer::timeout, counter,
                         [counter, count = 0]() mutable {
            counter->setText(QStringLiteral("界面计数：%1").arg(++count));
        });
        timer->start(100);

        QObject::connect(start, &QPushButton::clicked, this,
                         [this, start, cancel, status, bar] {
            start->setEnabled(false);
            cancel->setEnabled(true);
            status->setText(QStringLiteral("状态：执行中"));

            // Worker 无父对象，才能移动到工作线程。
            auto *worker = new Worker;
            worker->moveToThread(&thread);
            QObject::connect(&thread, &QThread::started,
                             worker, &Worker::doWork);

            // 接收对象 bar 属于 GUI 线程，更新会排队到 GUI 线程。
            QObject::connect(worker, &Worker::progress,
                             bar, &QProgressBar::setValue);
            QObject::connect(worker, &Worker::finished, this,
                             [status, cancel](bool cancelled) {
                status->setText(cancelled ? QStringLiteral("状态：已取消")
                                          : QStringLiteral("状态：已完成"));
                cancel->setEnabled(false);
            });
            QObject::connect(worker, &Worker::finished,
                             &thread, &QThread::quit);
            // Qt 支持在线程结束阶段处理这个延迟删除。
            QObject::connect(&thread, &QThread::finished,
                             worker, &QObject::deleteLater);
            thread.start();
        });

        QObject::connect(cancel, &QPushButton::clicked, this,
                         [this, status, cancel] {
            // 线程安全的协作取消请求；Worker 主动检查后返回。
            thread.requestInterruption();
            status->setText(QStringLiteral("状态：正在取消……"));
            cancel->setEnabled(false);
        });
        qDebug() << "GUI thread:" << QThread::currentThread();
    }

    ~Window() override
    {
        // 先请求任务退出，再等待线程结束，避免销毁运行中的 QThread。
        thread.requestInterruption();
        thread.quit();
        thread.wait();
    }

private:
    QThread thread;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Window window;
    window.show();
    return app.exec();
}

// 本文件定义了带 Q_OBJECT 的类，让 qmake 生成并包含 moc 代码。
#include "main.moc"
