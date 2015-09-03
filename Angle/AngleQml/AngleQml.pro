#-------------------------------------------------
#
# Project created by QtCreator 2015-09-02T12:32:45
#
#-------------------------------------------------

QT       += qml quick

QT       -= gui

TARGET = AngleQml
TEMPLATE = lib

DEFINES += ANGLEQML_LIBRARY

SOURCES += angleqmlrendersurface.cpp

HEADERS += angleqmlrendersurface.h\
        angleqml_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
