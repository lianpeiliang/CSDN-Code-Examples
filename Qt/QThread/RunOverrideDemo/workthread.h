#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QThread>

class WorkThread : public QThread
{
    Q_OBJECT

public:
signals:
    void progressChanged(int value);

protected:
    void run() override;
};

#endif // WORKTHREAD_H
