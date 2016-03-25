#ifndef STREAMSERVICEPLUGIN_H
#define STREAMSERVICEPLUGIN_H

#include <extensionsystem/iplugin.h>
#include "../uavobjects/uavobject.h"

//#include <coreplugin/iconfigurableplugin.h>

#include <QtPlugin>

class StreamServicePlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "Openpilot.StreamService")

public:
    StreamServicePlugin();
    ~StreamServicePlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    void shutdown();

public slots:

    void objectUpdated(UAVObject *pObj);

};

#endif // STREAMSERVICEPLUGIN_H
