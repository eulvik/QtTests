QT += core quick qml widgets
TARGET = AngleQmlRunner
CONFIG += console
CONFIG -= app_bundle

ARCH_EXT=64
TEMPLATE = app

SOURCES += main.cpp


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../AngleQml/release/ -lAngleQml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../AngleQml/debug/ -lAngleQml

INCLUDEPATH += $$PWD/../AngleQml
DEPENDPATH += $$PWD/../AngleQml
