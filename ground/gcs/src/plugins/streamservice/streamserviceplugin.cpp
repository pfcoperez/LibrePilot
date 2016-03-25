#include "streamserviceplugin.h"

#include <iostream>

#include <QJsonObject>
#include <QJsonDocument>

#include "extensionsystem/pluginmanager.h"
#include "../uavobjects/uavobjectmanager.h"
#include "../uavobjects/uavdataobject.h"

StreamServicePlugin::StreamServicePlugin()
{
    ;
}

StreamServicePlugin::~StreamServicePlugin()
{
    ;
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

}

void StreamServicePlugin::shutdown()
{

}

void StreamServicePlugin::objectUpdated(UAVObject *pObj)
{
    QJsonObject qtjson;
    pObj->toJson(qtjson);

    QJsonDocument jsonDoc(qtjson);
    QString strJson(jsonDoc.toJson(QJsonDocument::Compact));
    std::cout << "\n\n\n-------------------------------> OBJECT Changed: " << strJson.toUtf8().constData() << "\n";
}
