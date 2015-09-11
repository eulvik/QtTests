QT += core quick qml widgets
TARGET = AngleQmlRunner
CONFIG += console
CONFIG -= app_bundle

ARCH_EXT=64
TEMPLATE = app

SOURCES += main.cpp

LIBS += -L$$PWD/../build/lib$${ARCH_EXT} -lAngleQml

INCLUDEPATH += $$PWD/../AngleQml
DEPENDPATH += $$PWD/../AngleQml
