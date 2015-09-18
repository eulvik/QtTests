#-------------------------------------------------
#
# Project created by QtCreator 2015-09-02T12:32:45
#
#-------------------------------------------------

QT       += qml quick gui

TARGET = AngleQml
TEMPLATE = lib

ARCH_EXT=64

INCLUDEPATH += $$[QT_INSTALL_PREFIX]/../src/qtbase/src/3rdparty/angle/src

DEFINES += ANGLEQML_LIBRARY

Release:LIBS += -llibEGL
Debug:LIBS += -llibEGLd

SOURCES += \
    rendercontrolwindow.cpp \
    rendercontrolnativeinterface.cpp

HEADERS +=\
    angleqml_global.h \
    rendercontrolwindow.h \
    rendercontrolnativeinterface.h

RESOURCES += \
    qml.qrc


DISTFILES += *.qml \
    fragmentshader.fsh \
    vertexshader.vsh

DESTDIR = $$PWD/../build/bin

