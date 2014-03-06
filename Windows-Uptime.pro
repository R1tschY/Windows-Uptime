#-------------------------------------------------
#
# Project created by QtCreator 2014-02-26T23:14:33
#
#-------------------------------------------------

VERSION = 0.0

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Windows-Uptime
TEMPLATE = app

LIBS            += -lWevtapi
QMAKE_CXXFLAGS  += -std=gnu++0x
QMAKE_LFLAGS    += -std=gnu++0x

# do not compress png resources
QMAKE_RESOURCE_FLAGS += -no-compress

# Schneller als die Standardimplementierung wenn Strings mit + verbunden werden
DEFINES *= QT_USE_QSTRINGBUILDER

# Version für Quelldateien
DEFINES += PACKAGE_VERSION=$$VERSION

# Find Boost library

INCLUDEPATH += $$BOOST_ROOT

# include what we need
QMAKE_CXXFLAGS += \
    -DWIN32_LEAN_AND_MEAN \
    -DNOCOMM \
    -DNOUSER

# compiling for Vista and up
QMAKE_CXXFLAGS += \
    -DNTDDI_VERSION=NTDDI_VISTA \
    -D_WIN32_WINNT=_WIN32_WINNT_VISTA \
    -DWINVER=_WIN32_WINNT_VISTA \
    -D_WIN32_IE=_WIN32_IE_IE80

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/winevt/eventlog.cpp \
    src/winevt/winexception.cpp \
    src/winevt/eventchanneliterator.cpp \
    src/winevt/event.cpp \
    src/winevt/eventiterator.cpp \
    src/winevt/variant.cpp \
    src/winevt/rendercontext.cpp \
    src/uptimerequest.cpp

HEADERS  += \
    src/mainwindow.h \
    src/winevt/eventlog.h \
    src/winevt/winexception.h \
    src/winevt/eventhandle.h \
    src/winevt/eventchanneliterator.h \
    src/winevt/event.h \
    src/winevt/eventiterator.h \
    src/winevt/errors.h \
    src/winevt/variant.h \
    src/winevt/rendercontext.h \
    src/uptimerequest.h

FORMS    +=

RESOURCES += \
    Resources.qrc
