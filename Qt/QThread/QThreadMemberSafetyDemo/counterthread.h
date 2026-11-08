#ifndef COUNTERTHREAD_H
#define COUNTERTHREAD_H

#include <QMutex>
#include <QThread>

class CounterThread : public QThread
{
    Q_OBJECT

public:
    explicit CounterThread(QObject *parent = nullptr);

    int readCounter() const;

public slots:
    void resetCounter();

protected:
    void run() override;

private:
    mutable QMutex counterMutex;
    int counterValue = 0;
};

#endif // COUNTERTHREAD_H
