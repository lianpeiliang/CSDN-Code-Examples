#ifndef GUIBLOCKINGDEMO_H
#define GUIBLOCKINGDEMO_H

#include <QWidget>

class QLabel;

class GuiBlockingDemo : public QWidget
{
public:
    explicit GuiBlockingDemo(QWidget *parent = nullptr);

private:
    void updateCounter();
    void blockMainThread();

    QLabel *counterLabel = nullptr;
    QLabel *statusLabel = nullptr;
    int counterValue = 0;
};

#endif // GUIBLOCKINGDEMO_H
