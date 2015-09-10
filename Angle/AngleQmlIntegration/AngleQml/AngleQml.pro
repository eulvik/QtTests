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

Release:LIBS += -llibEGL
Debug:LIBS += -llibEGLd

DEFINES += ANGLEQML_LIBRARY

SOURCES += \
    angleqmlnativeinterface.cpp \
    angleqmlrenderwindow.cpp

HEADERS +=\
        angleqml_global.h \
    angleqmlnativeinterface.h \
    angleqmlrenderwindow.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    qml.qrc


DISTFILES += *.qml \

target.path = $$PWD/../lib$${ARCH_EXT}
INSTALLS += target