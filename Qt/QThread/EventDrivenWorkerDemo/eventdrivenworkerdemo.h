#ifndef EVENTDRIVENWORKERDEMO_H
#define EVENTDRIVENWORKERDEMO_H

#include <QThread>
#include <QWidget>

class EventWorker;
class QLabel;
class QProgressBar;
class QPushButton;
class QTimer;

class EventDrivenWorkerDemo : public QWidget
{
    Q_OBJECT

public:
    explicit EventDrivenWorkerDemo(QWidget *parent = nullptr);
    ~EventDrivenWorkerDemo() override;

signals:
    void startRequested(int totalSteps, int intervalMs);
    void cancelRequested();

private slots:
    void startTask();
    void cancelTask();
    void updateProgress(int value);
    void onTaskFinished(bool cancelled);
    void updateHeartbeat();

private:
    QThread workerThread;
    EventWorker *worker = nullptr;
    QLabel *statusLabel = nullptr;
    QLabel *heartbeatLabel = nullptr;
    QProgressBar *progressBar = nullptr;
    QPushButton *startButton = nullptr;
    QPushButton *cancelButton = nullptr;
    QTimer *heartbeatTimer = nullptr;
    int heartbeatCount = 0;
};

#endif // EVENTDRIVENWORKERDEMO_H
