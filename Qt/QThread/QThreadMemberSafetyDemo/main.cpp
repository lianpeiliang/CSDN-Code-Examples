#include <QApplication>

#include "qthreadmembersafetydemo.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QThreadMemberSafetyDemo window;
    window.show();
    return app.exec();
}
