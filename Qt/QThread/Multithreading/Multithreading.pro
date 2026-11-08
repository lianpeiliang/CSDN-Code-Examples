QT += widgets

CONFIG += c++17
TEMPLATE = app
TARGET = Multithreading

SOURCES += \
    main.cpp \
    worker.cpp \
    window.cpp

HEADERS += \
    worker.h \
    window.h
