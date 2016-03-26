QT       += network widgets

TARGET = StreamServicePlugin
TEMPLATE = lib

include(../../plugin.pri)
include(../../plugins/coreplugin/coreplugin.pri)

#TODO
include(../../plugins/uavtalk/uavtalk.pri)
include(../../plugins/uavobjects/uavobjects.pri)

SOURCES += streamserviceplugin.cpp \
    streamservicepluginoptions.cpp

HEADERS += streamserviceplugin.h \
    streamservicepluginoptions.h

OTHER_FILES +=

DISTFILES += \
    StreamServicePlugin.pluginspec

FORMS += \
    streamservicepluginoptions.ui
