QT       += network widgets

TARGET = StreamServicePlugin
TEMPLATE = lib

include(../../plugin.pri)
include(../../plugins/coreplugin/coreplugin.pri)

#TODO
include(../../plugins/uavtalk/uavtalk.pri)
include(../../plugins/uavobjects/uavobjects.pri)

SOURCES += streamserviceplugin.cpp

HEADERS += streamserviceplugin.h

OTHER_FILES +=

DISTFILES += \
    StreamServicePlugin.pluginspec
