#include "streamserviceplugin.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>

#include "extensionsystem/pluginmanager.h"
#include "../uavobjects/uavobjectmanager.h"
#include "../uavobjects/uavdataobject.h"

StreamServicePlugin::StreamServicePlugin()
{
    pServer = new QTcpServer(this);
}

StreamServicePlugin::~StreamServicePlugin()
{
    if(pServer == Q_NULLPTR) return;
    if(pServer->isListening()) pServer->close();
    delete pServer;
}

bool StreamServicePlugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorString);

    ExtensionSystem::PluginManager *pm = ExtensionSystem::PluginManager::instance();

    UAVObjectManager *objManager = pm->getObject<UAVObjectManager>();

    Q_ASSERT(objManager);

    QList< QList<UAVDataObject *> > objList = objManager->getDataObjects();
    foreach(QList<UAVDataObject *> list, objList) {
        foreach(UAVDataObject * obj, list) {
            connect(obj, SIGNAL(objectUpdated(UAVObject *)), this, SLOT(objectUpdated(UAVObject *)));
        }
    }

    return true;
}

void StreamServicePlugin::extensionsInitialized()
{
    if(!pServer->listen(QHostAddress::Any, port))
    {
        QMessageBox::critical(Q_NULLPTR, tr("Stream Service"), tr("Couldn't start server!"));
        //delete pServer;
    }

    connect(pServer, SIGNAL(newConnection()), this, SLOT(clientConnected()));
}

void StreamServicePlugin::shutdown()
{
    pServer->pauseAccepting();
    foreach (QTcpSocket * pClient, activeClients) {
        pClient->disconnectFromHost();
    }
}

void StreamServicePlugin::objectUpdated(UAVObject *pObj)
{
    QJsonObject qtjson;
    pObj->toJson(qtjson);

    QJsonDocument jsonDoc(qtjson);
    QString strJson(jsonDoc.toJson(QJsonDocument::Compact));

    foreach (QTcpSocket * pClient, activeClients)
        if(pClient->isOpen())
            if(pClient->write(strJson.toUtf8().constData(), strJson.length()))
                pClient->flush();

}

void StreamServicePlugin::clientConnected()
{
    if(pServer == Q_NULLPTR) return;
    QTcpSocket *pending = pServer->nextPendingConnection();
    connect(pending, SIGNAL(disconnected()), this, SLOT(clientDisconnected));
    activeClients.append(pending);
}

void StreamServicePlugin::clientDisconnected()
{
    activeClients.removeAll((QTcpSocket *)sender());
}
