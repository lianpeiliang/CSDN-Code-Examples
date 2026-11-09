QT += widgets

CONFIG += c++17
TEMPLATE = app
TARGET = EventDrivenWorkerDemo

SOURCES += \
    main.cpp \
    eventdrivenworkerdemo.cpp \
    eventworker.cpp

HEADERS += \
    eventdrivenworkerdemo.h \
    eventworker.h
