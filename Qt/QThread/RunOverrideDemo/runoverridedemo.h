#ifndef RUNOVERRIDEDEMO_H
#define RUNOVERRIDEDEMO_H

#include <QWidget>

#include "workthread.h"

class QLabel;
class QProgressBar;
class QPushButton;

class RunOverrideDemo : public QWidget
{
    Q_OBJECT

public:
    explicit RunOverrideDemo(QWidget *parent = nullptr);
    ~RunOverrideDemo() override;

private slots:
    void startWork();
    void cancelWork();
    void onThreadFinished();

private:
    WorkThread workerThread;
    QLabel *statusLabel = nullptr;
    QProgressBar *progressBar = nullptr;
    QPushButton *startButton = nullptr;
    QPushButton *cancelButton = nullptr;
    bool cancellationRequested = false;
};

#endif // RUNOVERRIDEDEMO_H
