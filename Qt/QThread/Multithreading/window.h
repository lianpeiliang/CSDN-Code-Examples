#ifndef WINDOW_H
#define WINDOW_H

#include <QThread>
#include <QWidget>

class QLabel;
class QProgressBar;
class QPushButton;

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = nullptr);
    ~Window() override;

public slots:
    // 按钮和定时器的槽函数都在 GUI 线程执行。
    void updateCounter();
    void startTask();
    void cancelTask();
    void onTaskFinished(bool cancelled);

private:
    // QThread 控制一个执行线程；这个 QThread 对象本身仍属于 GUI 线程。
    QThread thread;
    QLabel *counterLabel = nullptr;
    QLabel *statusLabel = nullptr;
    QProgressBar *progressBar = nullptr;
    QPushButton *startButton = nullptr;
    QPushButton *cancelButton = nullptr;
    int counterValue = 0;
};

#endif // WINDOW_H
