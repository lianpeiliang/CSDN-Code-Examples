QT += widgets

CONFIG += c++17
TEMPLATE = app
TARGET = QThreadMemberSafetyDemo

SOURCES += \
    main.cpp \
    qthreadmembersafetydemo.cpp \
    counterthread.cpp

HEADERS += \
    qthreadmembersafetydemo.h \
    counterthread.h
