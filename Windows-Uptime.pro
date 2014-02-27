#-------------------------------------------------
#
# Project created by QtCreator 2014-02-26T23:14:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Windows-Uptime
TEMPLATE = app

LIBS            += -lWevtapi
QMAKE_CXXFLAGS  += -std=gnu++0x
QMAKE_LFLAGS    += -std=gnu++0x

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/eventlog.cpp

HEADERS  += \
    src/mainwindow.h \
    src/winversion.h \
    src/eventlog.h

FORMS    +=
