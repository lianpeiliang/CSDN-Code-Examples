#include <QApplication>

#include "guiblockingdemo.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    GuiBlockingDemo window;
    window.show();
    return app.exec();
}
