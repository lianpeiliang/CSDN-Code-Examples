#ifndef EVENTWORKER_H
#define EVENTWORKER_H

#include <QObject>

class QTimer;

class EventWorker : public QObject
{
    Q_OBJECT

public:
    explicit EventWorker(QObject *parent = nullptr);

public slots:
    void initialize();
    void startTask(int totalSteps, int intervalMs);
    void cancelTask();

private slots:
    void processOneStep();

signals:
    void progressChanged(int value);
    void taskFinished(bool cancelled);

private:
    QTimer *timer = nullptr;
    int currentStep = 0;
    int totalSteps = 0;
    bool running = false;
};

#endif // EVENTWORKER_H
