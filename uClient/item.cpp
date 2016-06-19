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
    QJsonDocument itmDoc;
    itmDoc = QJsonDocument::fromJson(json->value("toLinks").toString().toUtf8());
    QJsonArray toar = itmDoc.array();
    if(!toar.empty()){
        for(int i = 0; i < toar.count(); i++){
            links chr;
            QJsonObject obj = toar.at(i).toObject();
            chr.char1 = obj["idChar1"].toInt();
            chr.val = obj["value"].toString();
            chr.char2 = obj["idChar2"].toInt();
            toLinks.append(chr);
        }
    }

    QJsonDocument itmDoc2;
    itmDoc2 = QJsonDocument::fromJson(json->value("fromLinks").toString().toUtf8());
    QJsonArray fromArr = itmDoc2.array();
    if(!fromArr.empty()){
        for(int i = 0; i < fromArr.count(); i++){
            links chr;
            QJsonObject obj = fromArr.at(i).toObject();
            chr.char1 = obj["idChar1"].toInt();
            chr.val = obj["value"].toString();
            chr.char2 = obj["idChar2"].toInt();
            fromLinks.append(chr);
        }
    }
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

bool Item::checkCompatibility(Item *itm1, Item *itm2)
{
    bool ret = true;
    for(int i = 0; i < itm1->fromLinks.size(); i++){
        for(int j = 0; i < itm2->toLinks.size(); i++){
            if(itm1->fromLinks.at(i).char1 == itm2->toLinks.at(j).char2){
                if(itm1->fromLinks.at(i).val != itm2->toLinks.at(j).val){
                    ret = false;
                    return ret;
                }
            }
        }
    }
    for(int i = 0; i < itm1->toLinks.size(); i++){
        for(int j = 0; i < itm2->fromLinks.size(); i++){
            if(itm1->toLinks.at(i).char2 == itm2->fromLinks.at(j).char1){
                if(itm1->toLinks.at(i).val != itm2->fromLinks.at(j).val){
                    ret = false;
                    return ret;
                }
            }
        }
    }
    return ret;
}
