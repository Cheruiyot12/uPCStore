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
    //qDebug() << json->value("itemPrice") << "------------------------------";
    price = json->value("itemPrice").toDouble();
}

void Item::addChars(QJsonArray *arr)
{
    chars.clear();
    if(!arr->empty()){
        for(int i = 0 ; i < arr->count(); i++){
            itemChars chr;
            QJsonObject obj = arr->at(i).toObject();
            chr.charname = obj["charName"].toString();
            chr.charValue = obj["charValue"].toString();
            chr.charUnits = obj["charUnits"].toString();
            chars.append(chr);
        }
        charsLoaded = true;
    }
}

QList<itemChars>* Item::getChars()
{
    return &chars;
}
