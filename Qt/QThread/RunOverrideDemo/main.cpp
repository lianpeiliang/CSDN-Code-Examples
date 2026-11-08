#include <QApplication>

#include "runoverridedemo.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    RunOverrideDemo window;
    window.show();
    return app.exec();
}
