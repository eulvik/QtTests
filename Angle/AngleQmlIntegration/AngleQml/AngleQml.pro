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
    angleqmlrenderwindow.cpp \
    rendercontrolwindow.cpp \
    rendercontrolnativeinterface.cpp

HEADERS +=\
        angleqml_global.h \
    angleqmlnativeinterface.h \
    angleqmlrenderwindow.h \
    rendercontrolwindow.h \
    rendercontrolnativeinterface.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    qml.qrc


DISTFILES += *.qml \

DESTDIR = $$PWD/../build/bin

target.path = $$PWD/../build/lib$${ARCH_EXT}
INSTALLS += target

header_files.files = $$HEADERS
header_files.path = $$PWD/../build/include
INSTALLS += header_files
