#include "item.h"

Item::Item(QObject *parent) : QObject(parent)
{

}

Item::Item(int nid, QString nname, int nprice, QObject *parent)
{
    id = nid;
    name = nname;
    price = nprice;
}

QJsonObject Item::toJson()
{
    QJsonObject itm;
    itm["itemId"] = id;
    itm["itemName"] = name;
    itm["itemPrice"] = price;

    return itm;
}

void Item::fromJson(QJsonObject *json)
{
    id = json->value("itemId").toInt();
    name = json->value("itemName").toString();
    price = json->value("itemPrice").toInt();
}
