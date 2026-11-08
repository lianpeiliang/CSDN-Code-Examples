#ifndef WORKER_H
#define WORKER_H

#include <QObject>

class Worker : public QObject
{
    Q_OBJECT

public slots:
    // 在线程启动后执行耗时任务；槽函数会在 Worker 所属的线程中调用。
    void doWork();

signals:
    // Worker 发出进度和结果，界面通过跨线程连接接收它们。
    void progress(int value);
    void finished(bool cancelled);
};

#endif // WORKER_H
