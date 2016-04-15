#include "itemgroup.h"

ItemGroup::ItemGroup(QObject *parent) : QObject(parent)
{

}

QJsonObject ItemGroup::toJson()
{
    QJsonObject newObj;
    newObj["groupId"] = id;
    newObj["groupName"] = name;

    return newObj;

}
void ItemGroup::fromJson(QJsonObject *json)
{
    id = json->value("groupId").toInt();
    name = json->value("groupName").toString();
}
