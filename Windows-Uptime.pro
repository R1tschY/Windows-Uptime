#-------------------------------------------------
#
# Project created by QtCreator 2014-02-26T23:14:33
#
#-------------------------------------------------

QT       += core gui widgets concurrent
CONFIG   += c++14
TARGET    = Windows-Uptime
TEMPLATE  = app
VERSION  = 0.1

LIBS            += -lWevtapi

CONFIG(release, debug|release) {
QMAKE_CXXFLAGS  += -flto
QMAKE_LFLAGS    += -flto
}

# do not compress png resources
QMAKE_RESOURCE_FLAGS += -no-compress

# Schneller als die Standardimplementierung wenn Strings mit + verbunden werden
DEFINES *= QT_USE_QSTRINGBUILDER

# Version für Quelldateien
DEFINES += PACKAGE_VERSION=\\\"$$VERSION\\\"

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
    src/winevt/eventiterator.cpp \
    src/winevt/variant.cpp \
    src/winevt/rendercontext.cpp \
    src/modules/module.cpp \
    src/modules/powerstatemodule.cpp \
    src/data/itemmodel.cpp \
    src/data/items.cpp \
    src/mouthview/daystablemodel.cpp \
    src/detailsview/detailsmodel.cpp \
    src/detailsview/detailsview.cpp \
    src/mouthview/timelineitemdelegate.cpp \
    src/timeutils.cpp \
    src/mouthview/overview.cpp

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
    src/utils.h \
    src/modules/module.h \
    src/modules/powerstatemodule.h \
    src/data/itemmodel.h \
    src/data/items.h \
    src/mouthview/daystablemodel.h \
    src/detailsview/detailsmodel.h \
    src/detailsview/detailsview.h \
    src/mouthview/timelineitemdelegate.h \
    src/timeutils.h \
    src/mouthview/overview.h

FORMS    +=

RESOURCES += \
    Resources.qrc
