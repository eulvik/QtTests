#-------------------------------------------------
#
# Project created by QtCreator 2014-11-18T12:15:48
#
#-------------------------------------------------

QT       += core gui widgets opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AngleHelloWorld
TEMPLATE = app


SOURCES += main.cpp\
    angleglwidget.cpp \
    geometryengine.cpp

HEADERS  += \
    angleglwidget.h \
    geometryengine.h

FORMS    +=

RESOURCES += \
    shaders.qrc \
    textures.qrc
