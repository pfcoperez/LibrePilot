#include "streamserviceplugin.h"

#include <iostream>

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

    std::cout  << "---------------- INIT MY PLUGIN --------------\n";

    UAVObjectManager *objManager = pm->getObject<UAVObjectManager>();

    QList< QList<UAVDataObject *> > objList = objManager->getDataObjects();
    std::cout << "------------>>" << objList.size() << "\n";
    foreach(QList<UAVDataObject *> list, objList) {
        foreach(UAVDataObject * obj, list) {
            connect(obj, SIGNAL(objectUpdated(UAVObject *)), this, SLOT(objectUpdated(UAVObject *)));
        }
    }

    //Q_ASSERT(objManager);

    return true;
}

void StreamServicePlugin::extensionsInitialized()
{
    if(!pServer->listen(QHostAddress::Any, port))
    {
        QMessageBox::critical(Q_NULLPTR, tr("Stream Service"), tr("Couldn't start server!"));
        delete pServer;
    }

    connect(pServer, SIGNAL(newConnection()), this, SLOT(clientConnected()));

    std::cout << ">CLIENT CAN CONNECT\n";
    std::cout.flush();
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

    //std::cout << "\n\n\n-------------------------------> OBJECT Changed: " << strJson.toUtf8().constData() << "\n";

    foreach (QTcpSocket * pClient, activeClients)
        if(pClient->isOpen()) {
            //std::cout << ">CLIENT WRITING";
            //std::cout.flush();
            pClient->write(strJson.toUtf8().constData(), strJson.length());
            pClient->flush();
        }

}

void StreamServicePlugin::clientConnected()
{
    if(pServer == Q_NULLPTR) return;
    QTcpSocket *pending = pServer->nextPendingConnection();
    connect(pending, SIGNAL(disconnected()), this, SLOT(clientDisconnected));
    activeClients.append(pending);
    std::cout << ">CLIENT CONNECTED: " << pending->peerAddress().toString().toUtf8().constData() << ":" << pending->peerPort() << "\n";
    std::cout.flush();
}

void StreamServicePlugin::clientDisconnected()
{
    activeClients.removeAll((QTcpSocket *)sender());
    std::cout << ">CLIENT DISCONNECTED\n";
    std::cout.flush();
}
