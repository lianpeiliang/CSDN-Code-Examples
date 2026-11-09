#include <QApplication>

#include "eventdrivenworkerdemo.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    EventDrivenWorkerDemo window;
    window.show();
    return app.exec();
}
