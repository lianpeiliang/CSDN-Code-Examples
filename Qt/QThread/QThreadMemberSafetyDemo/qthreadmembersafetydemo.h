#ifndef QTHREADMEMBERSAFETYDEMO_H
#define QTHREADMEMBERSAFETYDEMO_H

#include <QWidget>

#include "counterthread.h"

class QLabel;
class QPushButton;
class QTimer;

class QThreadMemberSafetyDemo : public QWidget
{
    Q_OBJECT

public:
    explicit QThreadMemberSafetyDemo(QWidget *parent = nullptr);
    ~QThreadMemberSafetyDemo() override;

private slots:
    void startCounter();
    void stopCounter();
    void refreshCounter();
    void onThreadFinished();

private:
    CounterThread counterThread;
    QLabel *counterLabel = nullptr;
    QLabel *statusLabel = nullptr;
    QPushButton *startButton = nullptr;
    QPushButton *stopButton = nullptr;
    QPushButton *resetButton = nullptr;
    QTimer *displayTimer = nullptr;
};

#endif // QTHREADMEMBERSAFETYDEMO_H
