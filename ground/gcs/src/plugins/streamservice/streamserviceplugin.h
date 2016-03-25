#ifndef STREAMSERVICEPLUGIN_H
#define STREAMSERVICEPLUGIN_H

#include <extensionsystem/iplugin.h>
#include "../uavobjects/uavobject.h"

//#include <coreplugin/iconfigurableplugin.h>

#include <QtPlugin>
#include <QTcpServer>
#include <QTcpSocket>

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

private slots:

    void clientConnected();
    void clientDisconnected();

private:

    QString bindAddress = "127.0.0.1";
    quint16 port = 7891;


    QTcpServer *pServer = Q_NULLPTR;
    QList<QTcpSocket *> activeClients;

};

#endif // STREAMSERVICEPLUGIN_H
