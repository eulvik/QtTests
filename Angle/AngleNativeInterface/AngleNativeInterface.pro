#-------------------------------------------------
#
# Project created by QtCreator 2014-11-19T11:52:42
#
#-------------------------------------------------

QT       += gui opengl

ARCH_EXT=64

TARGET = AngleNativeInterface
TEMPLATE = lib

DEFINES += ANGLENATIVEINTERFACE_LIBRARY

INCLUDEPATH += $$[QT_INSTALL_PREFIX]/../src/qtbase/src/3rdparty/angle/src

SOURCES += anglenativeinterface.cpp \
    rendersurface.cpp \
    geometryengine.cpp \
    angleglscene.cpp

HEADERS += anglenativeinterface.h\
        anglenativeinterface_global.h \
    mouseevent.h \
    rendersurface.h\
    geometryengine.h \
    angleglscene.h

RESOURCES += \
    shaders.qrc \
    textures.qrc

OTHER_FILES += fshader.glsl \
            vshader.glsl

target.path = $$PWD/../lib$${ARCH_EXT}
INSTALLS += target
